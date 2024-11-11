#pragma once

#include "StdAfx.h"
#include "ADSKMyDbReactor.h"

class CDocData {  // NOLINT(cppcoreguidelines-special-member-functions)
	ADSKMyDbReactor* mpDbReactor;
public:
	CDocData();
	CDocData(const CDocData& data);
	~CDocData();

	void attachDatabase(AcDbDatabase* pDb);
	void detachDatabase() const;
};
