#include "ADSRVisualizationControl.h"


double ADSRVisualizationControl::convertToGraphicX(double value) {
	double min = (double) this->mRECT.L;
	double distance = (double) this->mRECT.W();
	return value * distance + min;
};
double ADSRVisualizationControl::convertToPercentX(double value) {
	double min = (double) this->mRECT.L;
	double position = value - min;
	double distance = (double) this->mRECT.W();
	return position / distance;
};
double ADSRVisualizationControl::convertToGraphicY(double value) {
	double min = (double) this->mRECT.T;
	double distance = (double) this->mRECT.H();
	// We use (1 - value) as the max value 1 is located on top of graphics and not bottom
	return (1 - value) * distance + min;
};
double ADSRVisualizationControl::convertToPercentY(double value) {
	double min = (double) this->mRECT.T;
	double position = value - min;
	double distance = (double) this->mRECT.H();
	// We return the 1 - distance as the value 1 is located on top of graphics and not bottom
	return 1 - position / distance;
};

bool ADSRVisualizationControl::Draw(IGraphics *pGraphics) {
	IColor color(255, 50, 200, 20);

	//Cycles
	pGraphics->DrawCircle(&color, convertToGraphicX(startPoint.x), convertToGraphicY(startPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(endPoint.x), convertToGraphicY(endPoint.y), 3, 0, true);

	//Lines
	pGraphics->DrawLine(&color,
		convertToGraphicX(startPoint.x), convertToGraphicY(startPoint.y),
		convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y),
		0, true);
	pGraphics->DrawLine(&color,
		convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y),
		convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y),
		0, true);
	pGraphics->DrawLine(&color,
		convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y),
		convertToGraphicX(endPoint.x), convertToGraphicY(endPoint.y),
		0, true);

	return true;
};

void ADSRVisualizationControl::initPoints() {
	startPoint.x = 0;
	startPoint.y = 0;
	attackPoint.x = 0.2;
	attackPoint.y = 1;
	retainPoint.x = 0.4;
	retainPoint.y = 0.8;
	endPoint.x = 0.7;
	endPoint.y = 0;
}