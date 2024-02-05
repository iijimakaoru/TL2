#include "TextureConverter.h"
#include <stringapiset.h>

void TextureConverter::ConvertTextureWICToDDS(
    const std::string& filePath, int numOptions, char* options[]) {
	LoadWICTextureFromFile(filePath);

	SaveDDSTextureToFile(numOptions, options);
}

void TextureConverter::OutputUsage() {
	printf("�摜�t�@�C����WIC�`������DDS�`���ɕϊ����܂��B\n");
	printf("\n");
	printf("TextureConvert [�h���C�u][�p�X][�t�@�C����]\n");
	printf("\n");
	printf("[�h���C�u][�p�X][�t�@�C����]�F�ϊ�������WIC�`���̉摜�t�@�C�����w�肵�܂��B\n");
	printf("\n");
	printf("[-ml level]�F�~�b�v���x�����w�肵�܂��B0���w�肷���1x1�܂ł̃t���~�b�v�}�b�v�`�F�[�����쐬���܂��B\n");
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath) {
	HRESULT result;

	std::wstring wfilePath = ConvertMultiByteStringToWideString(filePath);

	result = DirectX::LoadFromWICFile(
	    wfilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metadata, scratchImage);
	assert(SUCCEEDED(result));

	SeparateFilePath(wfilePath);
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString) {
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	std::wstring wString;
	wString.resize(filePathBufferSize);

	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath) {
	size_t pos1;
	std::wstring exceptExt;

	pos1 = filePath.rfind('.');

	if (pos1 != std::wstring::npos) {
		fileExt = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		exceptExt = filePath.substr(0, pos1);
	} else {
		fileExt = L"";
		exceptExt = filePath;
	}

	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		directoryPath = exceptExt.substr(0, pos1 + 1);
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		directoryPath = exceptExt.substr(0, pos1 + 1);
		fileName = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	directoryPath = L"";
	fileName = exceptExt;
}

void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[]) {
	HRESULT result;

	size_t mipLevel = 0;

	for (int i = 0; i < numOptions; i++) {
		if (std::string(options[i]) == "-ml") {
			mipLevel = std::stoi(options[i + 1]);
			break;
		}
	}

	DirectX::ScratchImage mipChain;
	// �~�j�}�b�v�쐬
	result = GenerateMipMaps(
	    scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(),
	    DirectX::TEX_FILTER_DEFAULT, mipLevel, mipChain);

	if (SUCCEEDED(result)) {
		scratchImage = std::move(mipChain);
		metadata = scratchImage.GetMetadata();
	}

	// ���k�`���ɕϊ�
	DirectX::ScratchImage converted;
	result = DirectX::Compress(
	    scratchImage.GetImages(), scratchImage.GetImageCount(), metadata,
	    DXGI_FORMAT_BC7_UNORM_SRGB,
	    DirectX::TEX_COMPRESS_BC7_QUICK | DirectX::TEX_COMPRESS_SRGB_OUT |
	        DirectX::TEX_COMPRESS_PARALLEL,
	    1.0f, converted);
	if (SUCCEEDED(result)) {
		scratchImage = std::move(converted);
		metadata = scratchImage.GetMetadata();
	}

	std::wstring filePath = directoryPath + fileName + L".dds";

	result = DirectX::SaveToDDSFile(
	    scratchImage.GetImages(), scratchImage.GetImageCount(), metadata, DirectX::DDS_FLAGS_NONE,
	    filePath.c_str());
	assert(SUCCEEDED(result));
}
