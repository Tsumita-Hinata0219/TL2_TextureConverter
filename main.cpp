#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cassert>
#include <Windows.h>

#include "Projects/TextureConverter/TextureConverter.h"

using namespace std;

const char* PAUSE = "pause";

// コマンドライン引数
enum Argment {
	KApplicationPath, // アプリケーションのパス
	kFilePath,		  // 渡されたファイルのパス

	NumArgment,
};


int main(int argc, char* argv[]) {

	// ───────── 引数指定なしなので、
	if (argc < NumArgment) {
		TextureConverter::OutputUsage();
		system(PAUSE); // 閉じる前にポーズしておく
		return 0;
	}

	// ───────── 引数が指定されているのでコンバート処理に入る
	// COM ライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	assert(SUCCEEDED(hr));

	// テクスチャコンバータクラスのインスタンス
	std::unique_ptr<TextureConverter> texConverter =
		std::make_unique<TextureConverter>();

	// オプション数
	int numOptions = argc - NumArgment;
	// オプション配列(ダブルポインタ)
	char** options = argv + NumArgment;
	// テクスチャ変換
	texConverter->ConvertTextureWIC_To_DDS(argv[kFilePath], numOptions, options);

	// COM ライブラリの終了
	CoUninitialize();

	return 0;
}