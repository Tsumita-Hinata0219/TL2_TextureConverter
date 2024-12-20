#pragma once

#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <filesystem>
#include <Windows.h>

#include "../../Externals/DirectXTex/DirectXTex.h"


/* テクスチャコンバータクラス */
class TextureConverter {

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureConverter() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureConverter() = default;

	/// <summary>
	/// 使用方法を出力する(表示)する
	/// </summary>
	static void OutputUsage();

	/// <summary>
	/// テクスチャをWICからDDSに変換する
	/// </summary>
	/// <param name="filePath"> ファイルパス </param>
	void ConvertTextureWIC_To_DDS(const std::string& filePath, int numOptions = 0, char* options[] = nullptr);

	
private:

	/// <summary>
	/// ワイド文字列への変換と分離
	/// </summary>
	/// <param name="filePath"></param>
	void ConvertAndSeparateFilePath(const std::string& filePath);

	/// <summary>
	/// テクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath"> ファイルパス </param>
	void LoadWICTextureFromFile();

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換
	/// </summary>
	/// <param name="mString"> マルチバイト文字列 </param>
	/// <returns> ワイド文字列 </returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

	/// <summary>
	/// フォルダパスとファイル名を分離する
	/// </summary>
	/// <param name="filePath"></param>
	void SeparateFilePath();

	/// <summary>
	/// 分離されたパス情報を出力
	/// </summary>
	void OutputSeparatedPathInfo();

	/// <summary>
	/// DDSテクスチャとしてファイル書き出し
	/// </summary>
	void SaveDDSTextureToFile(int numOptions, char* options[], const std::wstring& outputDirectory);


private:

	// 画像の情報
	DirectX::TexMetadata metaData_{};
	// 画像のイメージコンテナ
	DirectX::ScratchImage scratchImage_{};

	// ファイルパス
	std::wstring wFilePath_{};
	// ディレクトリ
	std::wstring directoryPath_{};
	// ファイル
	std::wstring fileName_{};
	// ファイル拡張子
	std::wstring fileExt_{};

};

