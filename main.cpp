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

	assert(argc >= NumArgment);

	// COM ライブラリの初期化
	HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	assert(SUCCEEDED(hr));


	// テクスチャコンバータクラスのインスタンス
	std::unique_ptr<TextureConverter> texConverter =
		std::make_unique<TextureConverter>();

	// テクスチャ変換
	texConverter->ConvertTextureWIC_To_DDS(argv[kFilePath]);


	// COM ライブラリの終了
	CoUninitialize();

	system(PAUSE); // 閉じる前にポーズしておく
	return 0;
}