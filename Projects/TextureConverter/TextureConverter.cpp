#include "TextureConverter.h"
using namespace std;


/// <summary>
/// テクスチャをWICからDDSに変換する
/// </summary>
void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath, int numOptions, char* options[])
{
	// ───── exeのディレクトリを取得する ─────
	wchar_t exePath[MAX_PATH];
	GetModuleFileNameW(nullptr, exePath, MAX_PATH); // exeファイルの絶対パスを取得
	filesystem::path exeDirectory = filesystem::path(exePath).remove_filename();
	// 出力ディレクトリ名を設定 (exeと同じディレクトリ内に"Converted"フォルダを作成)
	wstring outputDirectory = exeDirectory / L"Converted";
	// 出力ディレクトリを作成
	namespace fs = filesystem;
	if (!fs::exists(outputDirectory)) {
		fs::create_directories(outputDirectory); // ディレクトリを再帰的に作成
	}


	// ───── 1 ワイド文字列への変換と分離
	ConvertAndSeparateFilePath(filePath);

	// ───── 2 テクスチャを読み込む
	LoadWICTextureFromFile();

	// ───── 3 DDS形式に変換して書き出す
	SaveDDSTextureToFile(numOptions, options, outputDirectory);
}


/// <summary>
/// 使用方法を出力する(表示)する
/// </summary>
void TextureConverter::OutputUsage()
{
	cout << "画像ファイルをWIC形式からDDS形式に変換します" << endl;
	cout << "ファイルを指定してください" << endl;
	cout << "TextureConverter [ドライブ:][パス][ファイル名]" << endl;
	cout << endl;
}


/// <summary>
/// ワイド文字列への変換とス分離
/// </summary>
void TextureConverter::ConvertAndSeparateFilePath(const std::string& filePath)
{
	// ───── ファイルパスをワイド文字列に変換
	wFilePath_ = ConvertMultiByteStringToWideString(filePath);

	// ───── フォルダパスとファイル名を分離する
	SeparateFilePath();

	// ───── 分離されたパス情報を出力
	OutputSeparatedPathInfo();
}


/// <summary>
/// テクスチャファイルの読み込み
/// </summary>
void TextureConverter::LoadWICTextureFromFile()
{
	// ───── テクスチャを読み込む
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath_.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));
}


/// <summary>
/// フォルダパスとファイル名を分離する
/// </summary>
void TextureConverter::SeparateFilePath()
{
	size_t pos1 = 0;
	wstring exceptExt;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos1 = wFilePath_.rfind('.');
	// 検索がヒットしたら
	if (pos1 != wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = wFilePath_.substr(pos1 + 1, wFilePath_.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = wFilePath_.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = wFilePath_;
	}
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != wstring::npos) {
		// 区切り文字がないのでファイル名のみとして扱う
		directoryPath_ = L"";
		fileName_ = exceptExt;
	}
}


/// <summary>
/// 分離されたパス情報を出力
/// </summary>
void TextureConverter::OutputSeparatedPathInfo()
{
	cout << "画像ファイルをWIC形式からDDS形式に変換します" << endl;
	cout << endl;

	// wstring をマルチバイト文字列に変換する関数
	auto WideStringToMultiByte = [](const std::wstring& wideString) -> std::string {
		if (wideString.empty()) return std::string();

		// 必要なバッファサイズを計算
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, nullptr, 0, nullptr, nullptr);
		if (bufferSize <= 0) return std::string();

		// バッファを確保して変換
		std::string multiByteString(bufferSize, '\0');
		WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), -1, &multiByteString[0], bufferSize, nullptr, nullptr);

		// 終端文字を削除して返す
		multiByteString.pop_back(); // Null terminator を削除
		return multiByteString;
		};

	// 各変数を UTF-8 に変換して cout で出力
	cout << "入力されたファイルパス : " << WideStringToMultiByte(wFilePath_) << endl;
	cout << "ディレクトリパス       : " << WideStringToMultiByte(directoryPath_) << endl;
	cout << "ファイル名             : " << WideStringToMultiByte(fileName_) << endl;
	cout << "ファイル拡張子         : " << WideStringToMultiByte(fileExt_) << endl;
	cout << endl;

	// 出力場所の表示
	cout << "'Converted'に出力されます" << endl;
	// 処理の進行状況のサンプル出力
	cout << "[進行状況] 変換中..." << endl;
}


/// <summary>
/// マルチバイト文字列をワイド文字列に変換
/// </summary>
std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字
	wstring wString;
	wString.resize(filePathBufferSize);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}


/// <summary>
/// DDSテクスチャとしてファイル書き出し
/// </summary>
void TextureConverter::SaveDDSTextureToFile(int numOptions, char* options[], const std::wstring& outputDirectory)
{
	HRESULT hr{};
	// 上下反転用の ScratchImage を用意
	DirectX::ScratchImage flippedImage;
	HRESULT flipResult = DirectX::FlipRotate(
		*scratchImage_.GetImage(0, 0, 0),
		DirectX::TEX_FR_FLIP_HORIZONTAL | 
		DirectX::TEX_FR_FLIP_VERTICAL, // X軸・Y軸両方反転
		flippedImage);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = move(flippedImage);
		metaData_ = scratchImage_.GetMetadata();
	}

	// ミップマップ生成
	// ミップマップレベル指定を検索
	size_t mipLevel = 0;
	for (int i = 0; i < numOptions; i++) {
		if (string(options[i]) == "-ml") {
			// ミップレベル指定
			mipLevel = stoi(options[i + 1]);
			break;
		}
	}
	DirectX::ScratchImage mipChain;
	hr = DirectX::GenerateMipMaps(
		scratchImage_.GetImages(), 
		scratchImage_.GetImageCount(), 
		scratchImage_.GetMetadata(),
		DirectX::TEX_FILTER_DEFAULT, 
		mipLevel, 
		mipChain);
	if (SUCCEEDED(hr)) {
		// イメージとメタデータを、ミップマップ版で置き換える
		scratchImage_ = move(mipChain);
		metaData_ = scratchImage_.GetMetadata();
	}

	// 圧縮形式に変換
	DirectX::ScratchImage converted;
	hr = DirectX::Compress(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(), 
		metaData_, 
		DXGI_FORMAT_BC7_UNORM_SRGB, 
		DirectX::TEX_COMPRESS_BC7_QUICK | 
		DirectX::TEX_COMPRESS_SRGB_OUT | 
		DirectX::TEX_COMPRESS_PARALLEL, 
		1.0f, converted);
	if (SUCCEEDED(hr)) {
		scratchImage_ = move(converted);
		metaData_ = scratchImage_.GetMetadata();
	}

	// 読み込んだテクスチャをSRGBとして扱う
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	// 出力ファイル名を設定する
	wstring filePath = outputDirectory + L"\\" + fileName_ + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}

