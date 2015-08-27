#include "integration/itk/itk.hxx"

/* ITK headers */
#include "itkImage.h"
#include "itkFixedArray.h"
#include "itkImportImageFilter.h"

#include "FilterCommon.hxx"
#include "EigenSato.hxx"
#include "EigenFrangi.hxx"

/* _orion_convert_ndarray3_to_itkImage
 *
 * [Internal function]
 *
 * Converts a 3D dimensional volume stored in the ndarray3* volume into an
 * itk::Image::Pointer.
 *
 * This allows for passing ndarray3* data into filters written in ITK.
 */
InternalImageType::Pointer _orion_convert_ndarray3_to_itkImage(ndarray3* vol) {
	/* Convert the pointer to the image data using an ImportImageFilter.
	 * See <http://www.itk.org/Wiki/ITK/Examples/IO/ImportImageFilter> for
	 * more information.  */
	typedef itk::ImportImageFilter< InternalPixelType, Dimension >    ImportFilterType;
	ImportFilterType::Pointer importFilter = ImportFilterType::New();

	ImportFilterType::SizeType  size;
	size[0] = vol->sz[0];
	size[1] = vol->sz[1];
	size[2] = vol->sz[2];

	ImportFilterType::IndexType start;
	start.Fill( 0 );

	ImportFilterType::RegionType region;
	region.SetIndex( start );
	region.SetSize(  size  );

	importFilter->SetRegion( region );

	double origin[ Dimension ];
	origin[0] = 0.0;    // X coordinate
	origin[1] = 0.0;    // Y coordinate
	origin[2] = 0.0;    // Z coordinate

	importFilter->SetOrigin( origin );

	double spacing[ Dimension ];
	spacing[0] = 1.0;    // along X direction
	spacing[1] = 1.0;    // along Y direction
	spacing[2] = 1.0;    // along Z direction

	importFilter->SetSpacing( spacing );

	const size_t number_of_pixels = size[0] * size[1] * size[2];
	InternalPixelType* buffer = vol->p;
	const bool itkImportImageFilter_will_own_buffer = false;
	importFilter->SetImportPointer( buffer, number_of_pixels,
			itkImportImageFilter_will_own_buffer );
	importFilter->Update();

	InternalImageType::Pointer vol_itk = importFilter->GetOutput();

	return vol_itk;
}

ndarray3* _orion_convert_itkImage_to_ndarray3( InternalImageType::Pointer vol_itk ) {
	WARN_UNIMPLEMENTED;
}

array_ndarray3* _orion_convert_itkFixedArray_to_array_ndarray( OutputImageArrayType arr_itk ) {
	/* get the size of the arr_itk and create an array_ndarray3* of the
	 * same size */
	OutputImageArrayType::SizeType sz = arr_itk.Size();
	array_ndarray3* arr_eig = array_new_ndarray3(sz);

	/* fill `arr_eig` with the ndarray3* volumes converted from `arr_eig_itk`'s
	 * elements */
	for( int i = 0; i < sz; sz++ ) {
		ndarray3* nd = _orion_convert_itkImage_to_ndarray3( arr_itk[i] );
		array_add_ndarray3( arr_eig, nd );
	}

	return arr_eig;
}

array_ndarray3* ApplyFilterSato( ndarray3* vol, float sigma ) {
	InternalImageType::Pointer vol_itk = _orion_convert_ndarray3_to_itkImage( vol );

	OutputImageArrayType arr_eig_itk  = ComputeSato( vol_itk, sigma );

	array_ndarray3* arr_eig = _orion_convert_itkFixedArray_to_array_ndarray( arr_eig_itk );
	return arr_eig;
}

array_ndarray3* ApplyFilterFrangi( ndarray3* vol, float sigma ) {
	InternalImageType::Pointer vol_itk = _orion_convert_ndarray3_to_itkImage( vol );

	OutputImageArrayType arr_eig_itk  = ComputeFrangi( vol_itk, sigma );

	array_ndarray3* arr_eig = _orion_convert_itkFixedArray_to_array_ndarray( arr_eig_itk );
	return arr_eig;
}

array_ndarray3* ApplyFilterSingleEigenvalue( ndarray3* vol, float sigma ) {
	WARN_UNIMPLEMENTED;
}