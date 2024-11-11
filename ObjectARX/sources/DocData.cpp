#include "StdAfx.h"
#include "common.h"

// ReSharper disable once CppInconsistentNaming
AcApDataManager<CDocData> DocVars;

CDocData::CDocData() : mpDbReactor(nullptr) {
}

CDocData::CDocData(const CDocData& data) : mpDbReactor(nullptr) {
}

CDocData::~CDocData() {
	delete mpDbReactor;
}

void CDocData::attachDatabase(AcDbDatabase* pDb) {
	if (mpDbReactor == nullptr)
		mpDbReactor = new ADSKMyDbReactor(pDb);
}

void CDocData::detachDatabase() const {
	if (mpDbReactor != nullptr) {
		mpDbReactor->Detach();
		delete mpDbReactor;
	}
}
