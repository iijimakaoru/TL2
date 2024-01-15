#include <cstdio>
#include <cstdlib>

#include <assert.h>

#include "TextureConverter.h"

#include<wrl.h>

enum Argument {
	kApplicationPath,
	kFilePath,

	NnmArgument
};

int main(int argc, char* argv[])
{
	assert(argc >= NnmArgument);

	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	TextureConverter converter;

	converter.ConvertTextureWICToDDS(argv[kFilePath]);

	CoUninitialize();

	system("pause");
	return 0;
}