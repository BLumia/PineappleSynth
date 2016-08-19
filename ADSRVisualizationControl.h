#pragma once

#include <vector>
#include "IControl.h"

class IPoint {
public:
	double x;
	double y;
	bool operator < (const IPoint& point) const { return (this->x < point.x); };
	bool operator >(const IPoint& point) const { return (this->x > point.x); };
};

class ADSRVisualizationControl : public IControl {
private:
	void initPoints();
	float attack, sustain, decay, release;
	IPoint startPoint, attackPoint, retainPoint, endPoint; //names should be changed.
	void updatePointPosition();
protected:
	double convertToGraphicX(double value);
	double convertToPercentX(double value);
	double convertToGraphicY(double value);
	double convertToPercentY(double value);
public:
	ADSRVisualizationControl(IPlugBase *pPlug, IRECT pR) : IControl(pPlug, pR) {
		initPoints();
	};
	~ADSRVisualizationControl() {};
	void setADSR(float att, float dec, float sus, float rel);

	bool IsDirty() { return true; }; //TODO: can't do that.

	bool Draw(IGraphics *pGraphics);
};