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
	if (argc < NnmArgument) {
		TextureConverter::OutputUsage();

		system("pause");
		return 0;
	}

	HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(result));

	TextureConverter converter;

	int numOptions = argc - NnmArgument;
	char** options = argv + NnmArgument;

	converter.ConvertTextureWICToDDS(argv[kFilePath], numOptions, options);

	CoUninitialize();

	return 0;
}