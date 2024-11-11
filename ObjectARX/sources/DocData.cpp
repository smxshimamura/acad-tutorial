#include "StdAfx.h"
#include "common.h"

// ReSharper disable once CppInconsistentNaming
AcApDataManager<CDocData> DocVars;

CDocData::CDocData() : pDbReactor(nullptr) {
}

CDocData::CDocData(const CDocData& data) : pDbReactor(nullptr) {
}

CDocData::~CDocData() {
	delete pDbReactor;
}
