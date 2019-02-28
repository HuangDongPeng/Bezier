#pragma once
#include "glm/glm.hpp"

class ISyncViews {
public:
	virtual void SyncViews(glm::vec3 concPos) = 0;
};

class ConcentrationDataManager
{
private:
	static ConcentrationDataManager* instance;

	float ***ConcData;


public:

	static ConcentrationDataManager* GetInstance() {
		if (instance == NULL)
			instance = new ConcentrationDataManager();

		return instance;
	}

	ConcentrationDataManager();
	~ConcentrationDataManager();

	void SyncViews(glm::vec3 concPos);

public:
	ISyncViews* modelView;
	ISyncViews* petViewer;
};

