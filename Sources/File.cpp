#define		_CRT_SECURE_NO_WARNINGS
#include	"PlatformSelector.h"
#include	"File.h"		// 定義
#include	"Allocator.h"	// 安全な解放

#ifdef _PLATFORM_DIRECTX
#include <stdio.h>		// FILE構造体
#include <stdarg.h>		// 可変引数を利用した関数を作るため
#endif

using namespace NAllocator;

CFile::CFile(){
	fileData = NULL;
	cur = 0;
	isOpen = false;
}

CFile::~CFile(){
	Close();
}

//==========================================================
// ファイルのオープン処理
// 引数
// 　_filePath : ファイルのパス(ファイルまでのパスと、拡張子も記述すること)
// 　_openType : ファイルオープン方法
// 戻り値
// 　true  ファイルデータ読み込み完了
// 　false ファイルオープンに失敗
bool CFile::Open(const char *_filePath, EOpenType _openType){
#ifdef _PLATFORM_DIRECTX
	FILE	*pFile_;
	char	openType_[5];	// 一応少し大きめに取っておく
	unsigned int fileSize_;

	// 既にファイルを開いていたら解放する。
	Close();

	// オープンタイプを文字列に変換する
	switch (_openType){
		case READ:
			sprintf(openType_, "r");
			break;
	}

	// ファイルオープン
	pFile_ = fopen(_filePath, openType_);	// ファイルのオープン
	if (pFile_ == NULL)						// エラー処理
		return false;

	// ファイルサイズの取得
	fseek(pFile_, 0, SEEK_END);		// ファイル終端へ移動
	fileSize_ = ftell(pFile_);		// カーソル位置(ファイルサイズ)の取得
	fseek(pFile_, 0, SEEK_SET);		// ファイルの先頭に戻す(後でそのまま使えるように)

	// ファイル内容格納バッファの確保
	fileData = new char[fileSize_];

	// ファイルをバッファーにコピー
	for (int i = 0; (fileData[i] = fgetc(pFile_)) != EOF; ++i){}

	fclose(pFile_);	// ファイル処理の終了

	// バッファのカーソル初期化
	cur = 0;

	// ファイルオープンフラグをtrueにする
	isOpen = true;

	return true;
#endif
}


void CFile::Close(){
	// ファイルをオープンしている時だけ解放する。
	if(!isOpen)
		return;

#ifdef _PLATFORM_DIRECTX
	DeleteArraySafely(fileData);	// バッファーの開放
	cur = 0;						// カーソルの初期化
#endif

	// オープンフラグをfalseにする
	isOpen = false;
}

//==========================================================
// 読み込み
// 文字
void CFile::Read(const char *_format, char *_dest){
	ScanFile(_format, _dest);
	CountReadCharNum();
}

// 整数
void CFile::Read(const char *_format, int *_dest){
	ScanFile(_format, _dest);
	CountReadCharNum();
}

// 浮動小数
void CFile::Read(const char *_format, float *_dest){
	ScanFile(_format, _dest);
	CountReadCharNum();
}

//==========================================================
// その他の操作
void CFile::SkipLine(){
	while (GetChar() != '\n'){}
}

//==========================================================
// 読み込みの際に使用する関数
// 書式付き読み込み
void CFile::ScanFile(const char *_format, ...){
	va_list arg;
	va_start(arg, _format);
	vsscanf(&fileData[cur], _format, arg);
	va_end(arg);
}

// 読み込んだ文字列をカウントする
void CFile::CountReadCharNum(){
	char c_;
	while (true){
		c_ = GetChar();
		if (c_ == ','){			// , の場合は、そのまま読み進める。
			break;
		}
		else if (c_ == '\n'){	// 行の最終列のデータを読み込んだ後に、行の読み飛ばしを行うと、次の行が読み飛ばされるため、その防止策としてこれ。
			cur--;
			break;
		}
	}
}

char CFile::GetChar(){
	char c_ = fileData[cur];	// 文字を取得
	if (c_ == EOF || c_ == '\0')
		return '\n';					// 判定処理の簡略化のため

	cur++;						// カーソルを進める
	return c_;
}