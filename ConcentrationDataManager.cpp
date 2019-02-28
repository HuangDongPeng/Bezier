#include "pch.h"
#include "ConcentrationDataManager.h"

ConcentrationDataManager* ConcentrationDataManager::instance = NULL;

ConcentrationDataManager::ConcentrationDataManager()
{

}


ConcentrationDataManager::~ConcentrationDataManager()
{
}

void ConcentrationDataManager::SyncViews(glm::vec3 concPos)
{
	if (modelView)
		modelView->SyncViews(concPos);
	if (petViewer)
		petViewer->SyncViews(concPos);
}