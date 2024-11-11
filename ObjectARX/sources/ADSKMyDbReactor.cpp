// ReSharper disable CppInconsistentNaming
#include "StdAfx.h"
#include "ADSKMyDbReactor.h"

ACRX_CONS_DEFINE_MEMBERS(ADSKMyDbReactor, AcDbDatabaseReactor, 1)

ADSKMyDbReactor::ADSKMyDbReactor(AcDbDatabase* pDb) : mpDatabase(pDb) {
	Attach(pDb);
}

ADSKMyDbReactor::~ADSKMyDbReactor() {
	Detach();
}

void ADSKMyDbReactor::Attach(AcDbDatabase* pDb) {
	Detach();
	mpDatabase = pDb;
	mpDatabase->addReactor(this);
}

void ADSKMyDbReactor::Detach() {
	if (mpDatabase != nullptr) {
		mpDatabase->removeReactor(this);
		mpDatabase = nullptr;
	}
}

AcDbDatabase* ADSKMyDbReactor::Subject() const {
	return mpDatabase;
}

bool ADSKMyDbReactor::IsAttached() const {
	return mpDatabase != nullptr;
}

void ADSKMyDbReactor::objectAppended(const AcDbDatabase* dwg, const AcDbObject* dbObj)
{
	AcDbDatabaseReactor::objectAppended(dwg, dbObj);
	acutPrintf(_T("Object added.\r\n"));
}

void ADSKMyDbReactor::objectErased(const AcDbDatabase* dwg, const AcDbObject* dbObj, Adesk::Boolean pErased)
{
	AcDbDatabaseReactor::objectErased(dwg, dbObj, pErased);
	acutPrintf(_T("Object erased.\r\n"));
}
