#pragma once

#ifdef __cplusplus

class ImageLabels {
public:
	ImageLabels();
	static ImageLabels& getInstance();
	void getImageLabels(wchar_t* filename, wchar_t* labels);
	void getUploadUrl(wchar_t* filename, wchar_t* url);
	
};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	void getImageLabels(wchar_t* filename, wchar_t* labels);
	void getUploadUrl(wchar_t* filename, wchar_t* url);


#ifdef __cplusplus
}
#endif