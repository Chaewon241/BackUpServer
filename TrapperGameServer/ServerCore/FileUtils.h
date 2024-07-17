#pragma once
#include <vector>
#include "Types.h"

/// <summary>
/// 경로를 읽어서 파일을 읽는 클래스
/// </summary>

class FileUtils
{
public:
	static Vector<BYTE>		ReadFile(const WCHAR* path);
	static String			Convert(string str);
};