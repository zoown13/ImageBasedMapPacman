#pragma once

#ifdef __cplusplus

class ImageLabels {
public:
	ImageLabels();
	static ImageLabels& getInstance();
	void getImageLabels(wchar_t* filename, wchar_t* labels);
};

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	void getImageLabels(wchar_t* filename, wchar_t* labels);


#ifdef __cplusplus
}
#endif