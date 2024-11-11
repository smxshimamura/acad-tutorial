#pragma once

#include "StdAfx.h"
#include "ADSKMyDbReactor.h"

class CDocData {  // NOLINT(cppcoreguidelines-special-member-functions)
public:
	ADSKMyDbReactor* pDbReactor;
	CDocData();
	CDocData(const CDocData& data);
	~CDocData();
};
