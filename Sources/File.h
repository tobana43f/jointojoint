//==========================================================
// csv形式のファイルを読み込みを行うクラス
#ifndef _INC_FILE_H
#define _INC_FILE_H

//==========================================================
// プラットフォームに依存しないファイル操作を行るクラス
// 文字列として、すべて展開する。
// メモ)
// 　クローズ処理は、解放処理で行う。
// 注意)
// 　csv形式しか対応していない
// 注意2)
// 　1データずつしか読み込めない。
// 　読み込んだ要素数がわからないので、必ず１つ読み込むことで数えている。
class CFile {
public:
	enum EOpenType{
		READ,
		//WRITE,	// 未対応
	};

	bool Open(const char *_filePath, EOpenType _openType);

	// 読み込み(型ごとに定義)
	// TODO:改良…1要素しか読み込まないからformatは固定じゃない？
	void Read(const char *_format, char *_dest);
	void Read(const char *_format, int *_dest);
	void Read(const char *_format, float *_deset);

	// 読み飛ばし
	void SkipLine();

private:
	bool			isOpen;	// 解放処理をする際に判断する
	char			*fileData;	// ファイルの全データ文字列確保先
	unsigned int	cur;		// 読込先カーソル(bufの添え字(ファイルの位置指定子の役割))
	
	void Close();

	// 読み込みを行う際に呼び出す
	void ScanFile(const char *_format, ...);
	void CountReadCharNum();
	char GetChar();


public:
	CFile();
	~CFile();
};

#endif