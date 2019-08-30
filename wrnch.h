#ifndef WRNCHAPI_H
#define WRNCHAPI_H

#include <QObject>
#include "model.h"
#include <QThread>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include "Visualizer.hpp"
#include <wrnch/engine.hpp>

class Wrnch : public QObject
{
    Q_OBJECT
public:
    Wrnch(Model*);
    ~Wrnch();

    int start();

    void DrawHands(wrnch::PoseEstimator::PoseHandsIter handsBegin,wrnch::PoseEstimator::PoseHandsIter handsEnd,
                   Visualizer& visualizer,
                   const unsigned int* bonePairs,
                   const unsigned int handNumJoints,
                   const unsigned int handNumBones);

    void renderHead(Visualizer &visualizer,unsigned int facialNumJoints, unsigned int facialNumBones,std::vector<unsigned int> facialBonePairs);
    
    void renderBody2D(Visualizer &visualizer,unsigned int bodyNumJoints, unsigned int bodyNumBones, std::vector<unsigned int> bodybBonePairs);
    
    void renderBody3D(Visualizer&,unsigned int,unsigned int,std::vector<unsigned int>,int);

    void renderBodyIk(Visualizer&,unsigned int,unsigned int,std::vector<unsigned int>,int);

    void renderHand(Visualizer &visualizer,unsigned int handNumJoints, unsigned int handNumBones, std::vector<unsigned int> handBonePairs);

    void setUpRenderBody(unsigned&,unsigned&,std::vector<unsigned int>&);

    void setUpRenderHead(unsigned&,unsigned&, std::vector<unsigned int>&);

    void setUpRenderHand(unsigned&,unsigned&, std::vector<unsigned int>&);

    std::vector<float> scalePosition(const float*,int);

    void json2csv(QString);
public slots:
    void run();

    int initialize();

    void handleThreadFinished();

private:
    Model *model;
    wrnch::JointDefinition outputFormat;
    wrnch::PoseEstimator poseEstimator;
    wrnch::PoseEstimatorOptions poseOptions;
    QThread* wrnchThread;

signals:
    void finished();
    void updateStatus(QString);
};

#endif // WRNCHAPI_H
