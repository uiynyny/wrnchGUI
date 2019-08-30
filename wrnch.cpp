#include "wrnch.h"
#include "Quaternion.hpp"
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include "json2csv.h"

Wrnch::Wrnch(Model *model)
{
    this->model=model;
    wrnchThread=new QThread();
    this->moveToThread(wrnchThread);
    connect(wrnchThread,SIGNAL(started()),this,SLOT(run()));
    connect(this,SIGNAL(finished()),this,SLOT(handleThreadFinished()));
}

Wrnch::~Wrnch()
{
    delete wrnchThread;
    delete model;
}

void Wrnch::setUpRenderBody(unsigned& bodyNumJoints,unsigned& bodyNumBones, std::vector<unsigned int>& bodyBonePairs)
{

    if(model->getUseIk()){
        wrnch::JointDefinition outputFormatIk = poseEstimator.GetHuman3DOutputFormat();
        bodyNumJoints=outputFormatIk.GetNumJoints();
        bodyNumBones=outputFormatIk.GetNumBones();
        bodyBonePairs.resize(bodyNumBones * 2);
        outputFormatIk.GetBonePairs(bodyBonePairs.data());
        poseOptions.SetEstimateMask(false);
        poseOptions.SetEstimate3D(true);
        poseOptions.SetUseIK(true);
        poseOptions.SetMainPersonId(wrnch_MAIN_ID_LARGEST);
    }else if(model->getRender3D()){
        wrnch::JointDefinition outputFormat3d = poseEstimator.GetHuman3DRawOutputFormat();
        bodyNumJoints=outputFormat3d.GetNumJoints();
        bodyNumBones=outputFormat3d.GetNumBones();
        bodyBonePairs.resize(bodyNumBones*2);
        outputFormat3d.GetBonePairs(bodyBonePairs.data());
        poseOptions.SetEstimate3D(true);
        poseOptions.SetMainPersonId(wrnch_MAIN_ID_ALL);
    }else{
        bodyNumJoints = poseEstimator.GetHuman2DOutputFormat().GetNumJoints();
        bodyNumBones = outputFormat.GetNumBones();
        bodyBonePairs.resize(bodyNumBones * 2);
        outputFormat.GetBonePairs(bodyBonePairs.data());
        poseOptions.SetEstimateMask(true);
    }
}

void Wrnch::setUpRenderHead(unsigned& facialNumJoints,unsigned& facialNumBones, std::vector<unsigned int>& facialBonePairs)
{
    wrnch::JointDefinition faceOutputFormat = poseEstimator.GetFaceOutputFormat();
    facialNumJoints = faceOutputFormat.GetNumJoints();
    facialNumBones=faceOutputFormat.GetNumBones();
    facialBonePairs.resize(facialNumBones * 2);
    faceOutputFormat.GetBonePairs(facialBonePairs.data());
    poseOptions.SetEstimateHead(true);
    poseOptions.SetEstimatePoseFace(true);
}

void Wrnch::setUpRenderHand(unsigned& handNumJoints, unsigned& handNumBones, std::vector<unsigned int>& handBonePairs)
{
    wrnch::JointDefinition handOutPutFormat = poseEstimator.GetHandOutputFormat();
    handNumJoints=handOutPutFormat.GetNumJoints();
    handNumBones=handOutPutFormat.GetNumBones();
    handBonePairs.resize(handNumBones*2);
    handOutPutFormat.GetBonePairs(handBonePairs.data());

    poseOptions.SetEstimateAllHandBoxes(true);
    poseOptions.SetEstimateHands(true);
    poseOptions.SetMainPersonId(wrnch_MAIN_ID_ALL);
}

std::vector<float> Wrnch::scalePosition(const float *positions, int numJoints2D)
{
    float minPos[]={1000,1000,1000};
    float maxPos[]={-1000,-1000,-1000};
    float scaleFactor[3];
    for(int d=0;d<3;d++){
        for(int i=d;i<numJoints2D*3;i+=3){
            if(positions[i]<minPos[d]) minPos[d]=positions[i];
            if(positions[i]>maxPos[d]) maxPos[d]=positions[i];
        }
        scaleFactor[d]=(maxPos[d]-minPos[d]);
    }
    float xyScaleFactor=(scaleFactor[1]>scaleFactor[0])? scaleFactor[1]:scaleFactor[0];
    std::vector<float> scalePos;
    scalePos.reserve(numJoints2D*3);
    for (int i=0; i<numJoints2D*3;i+=3) {
        scalePos.push_back(0.5+(positions[i]-(minPos[0]+maxPos[0])*0.5)/xyScaleFactor);
        scalePos.push_back(1.0-(positions[i+1]-minPos[1])/xyScaleFactor);
        scalePos.push_back(0.5*(1-(positions[i+2]-minPos[2])/scaleFactor[2]));
    }
    return scalePos;
}

void Wrnch::renderHead(Visualizer &visualizer,unsigned int facialNumJoints, unsigned int facialNumBones,std::vector<unsigned int> facialBonePairs)
{
    for (wrnch::PoseEstimator::PoseHeadIter it = poseEstimator.PoseHeadsBegin();it < poseEstimator.PoseHeadsEnd();it++)
    {
        wrnch::PoseHeadView pose = *it;
        wrnch::Box3dView boundingBox = pose.GetFaceBoundingBox();
        visualizer.DrawBox(boundingBox.GetMinX(),boundingBox.GetMinY(),boundingBox.GetWidth(),boundingBox.GetHeight(),WRNCH_BLUE,2.f);
    }
    for (wrnch::PoseEstimator::PoseFaceIter it = poseEstimator.PoseFacesBegin();it < poseEstimator.PoseFacesEnd();it++)
    {
        wrnch::PoseFaceView face = *it;
        visualizer.DrawPoints(face.GetLandmarks(), facialNumJoints, WRNCH_BLUE, 2.0f);
        visualizer.DrawLines(face.GetLandmarks(),&facialBonePairs[0],facialNumBones,WRNCH_BLUE,1);

        wrnch::ArrowView arrow = face.GetFaceArrow();
        visualizer.DrawArrow(arrow.GetBaseX(), arrow.GetBaseY(), arrow.GetTipX(), arrow.GetTipY());
    }
}

void Wrnch::renderBody2D(Visualizer &visualizer,unsigned int bodyNumJoints, unsigned int bodyNumBones, std::vector<unsigned int> bodyBonePairs)
{
    for (wrnch::PoseEstimator::Humans2dIter it = poseEstimator.Humans2dBegin(); it < poseEstimator.Humans2dEnd(); it++)
    {
        wrnch::Pose2dView pose = *it;
        visualizer.DrawPoints(pose.GetJoints(), bodyNumJoints);
        visualizer.DrawLines(pose.GetJoints(), &bodyBonePairs[0], bodyNumBones);
    }
}

void Wrnch::renderHand(Visualizer &visualizer,unsigned int handNumJoints, unsigned int handNumBones, std::vector<unsigned int> handBonePairs)
{
    DrawHands(poseEstimator.LeftHandsBegin(), poseEstimator.LeftHandsEnd(), visualizer, &handBonePairs[0], handNumJoints, handNumBones);
    DrawHands(poseEstimator.RightHandsBegin(), poseEstimator.RightHandsEnd(), visualizer, &handBonePairs[0], handNumJoints, handNumBones);
    for(wrnch::PoseEstimator::HandBoxIter it=poseEstimator.HandBoxesBegin();it!=poseEstimator.HandBoxesEnd();it++){
        wrnch::HandBoxPairView pair=*it;
        wrnch::Box2dView leftHandBox = pair.GetLeft();
        wrnch::Box2dView rightHandBox = pair.GetRight();
        visualizer.DrawBox(leftHandBox.GetMinX(),leftHandBox.GetMinY(),leftHandBox.GetWidth(),leftHandBox.GetHeight());
        visualizer.DrawBox(rightHandBox.GetMinX(),rightHandBox.GetMinY(),rightHandBox.GetWidth(),rightHandBox.GetHeight());
    }
}

int Wrnch::initialize(){
    emit updateStatus("Initializing network...");

    //get model directory
    QByteArray ba=model->getModel_dir().toLocal8Bit();
    const char* modelDirectory = ba.data();

    //parameters
    wrnch::PoseParams params;
    params.SetBoneSensitivity(wrSensitivity_HIGH);
    params.SetJointSensitivity(wrSensitivity_HIGH);
    params.SetEnableTracking(true);
    params.SetTrackerKind(wrTrackerKind::wrTrackerKind_V2);

    // Set network input resolution.
    // Different resolutions may impact performace depending on the size and aspect ratio of the
    // input image. The values used here correspond to a 16:9 aspect ratio.
    // Note: If these parameters are not set, the default resolution for the model will be used
    params.SetPreferredNetHeight2d(model->getResolution_height());
    params.SetPreferredNetWidth2d(model->getResolution_width());

    // Default skeleton for pose2d is j20 but j23 provides better bone pairs for visualization. See
    // the wrJointDefinitions_sample which prints the available definitions, joints and bone pairs
    if(model->getBodyRender()){
        QByteArray body= model->getBodyType().toLocal8Bit();
        const char* bodyType=body.data();
        outputFormat = wrnch::JointDefinitionRegistry::Get(bodyType);
    }
    //initialize post estimator
    try {
        if(model->getBodyRender()) {
            poseEstimator.Initialize(modelDirectory, model->getGpu(), params, outputFormat);
        }else{
            poseEstimator.Initialize(modelDirectory);
        }
        if(model->getHeadRender()) {
            poseEstimator.InitializeHead(modelDirectory);
        }
        if(model->getHandRender()){
            poseEstimator.InitializeHands2D(modelDirectory);
        }
        if(model->getRender3D()||model->getUseIk()){
            poseEstimator.Initialize3D(modelDirectory);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what()<<std::endl;
        return -1;
    }
    catch (...) {
        std::cerr << "Unknown error encountered when initializing"<<std::endl;
        return -1;
    }

    emit updateStatus("Initializing done");
    return 0;
}

void Wrnch::renderBody3D(Visualizer &visualizer,unsigned int bodyNumJoints, unsigned int bodyNumBones, std::vector<unsigned int> bodyBonePairs,int rotationY)
{
    for(wrnch::Pose3dView pose: poseEstimator.Humans3dRaw()){
        std::vector<float> positions;
        const float* pos=pose.GetPositions();
        visualizer.DrawLines3D(pos,bodyNumJoints,bodyBonePairs,bodyNumBones,0,rotationY,0);
    }
}

void Wrnch::renderBodyIk(Visualizer &visualizer, unsigned int bodyNumJoints, unsigned int bodyNumBones, std::vector<unsigned int> bodyBonePairs, int rotationY)
{
    for(wrnch::Pose3dView pose: poseEstimator.Humans3d()){
        const float* rotations=pose.GetRotations();
        const float* positions = pose.GetPositions();
        std::vector<float> scalePos=scalePosition(positions,bodyNumJoints);
        visualizer.DrawLines3D(scalePos.data(),bodyNumJoints,bodyBonePairs,bodyNumBones,0,rotationY,0);
        std::vector<float> x_axes;
        std::vector<float> y_axes;
        std::vector<float> z_axes;
        for(unsigned int rot_idx=0;rot_idx<bodyNumJoints*4;rot_idx+=4){
            Quaternion currentQ(rotations[rot_idx],rotations[rot_idx+1],rotations[rot_idx+2],rotations[rot_idx+3]);
            float x_axis[3]={1,0,0};
            float y_axis[3]={0,1,0};
            float z_axis[3]={0,0,1};
            currentQ.rotate(x_axis);
            currentQ.rotate(y_axis);
            currentQ.rotate(z_axis);

            float rotationAngleYRad=detail::deg2Rad(rotationY);
            Quaternion userQ(0,sin(rotationAngleYRad/2),0,cos(rotationAngleYRad/2));
            userQ.rotate(x_axis);
            userQ.rotate(y_axis);
            userQ.rotate(z_axis);

            x_axes.push_back(x_axis[0]);
            x_axes.push_back(x_axis[1]);
            y_axes.push_back(y_axis[0]);
            y_axes.push_back(y_axis[1]);
            z_axes.push_back(z_axis[0]);
            z_axes.push_back(z_axis[1]);
        }
        visualizer.DrawArrows3D(scalePos.data(),bodyNumJoints,x_axes,y_axes,z_axes,0,rotationY,0);

    }
}

void Wrnch::run()
{
    initialize();

    cv::VideoCapture cap;
    Visualizer visualizer;
    cv::VideoWriter video;
    cv::Mat frame;
    int rotationY=0;

    switch (model->getType()) {
    case Model::Type::CAM:
        cap.open(model->getCam_index());
        break;
    case Model::Type::FILE:
        cap.open(model->getFile().toStdString());
        break;
    }
    if (!cap.isOpened()) {
        std::cerr<< "Error opening video stream or file\n"<<std::endl;
        return;
    }

    if(model->getVideo()){
        double fps=cap.get(cv::CAP_PROP_FPS);
        double height=cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        double width=cap.get(cv::CAP_PROP_FRAME_WIDTH);

        video.open((model->getJson()+".avi").toStdString(),cv::VideoWriter::fourcc('M','J','P','G'),fps,cv::Size(width,height));
    }

    unsigned bodyNumJoints=0, bodyNumBones = 0; std::vector<unsigned int> bodyBonePairs;
    if(model->getBodyRender())
        setUpRenderBody(bodyNumJoints,bodyNumBones,bodyBonePairs);

    unsigned int facialNumJoints=0, facialNumBones=0; std::vector<unsigned int> facialBonePairs;
    if(model->getHeadRender())
        setUpRenderHead(facialNumJoints, facialNumBones, facialBonePairs);

    unsigned int handNumJoints=0, handNumBones=0; std::vector<unsigned int> handBonePairs;
    if(model->getHandRender()){
        setUpRenderHand(handNumJoints, handNumBones, handBonePairs);
    }

    std::ofstream out,outBody2d,outHand,outHead,outBody3d,outIk;
    if(!model->getJson().isEmpty()){
        std::string outputFile=model->getJson().toStdString();
        out.open(outputFile+".json");
        out<<"{\"frames\":[";
        if(model->getBodyRender()){
            outBody2d.open(outputFile+"_body2d.json");
            outBody2d<<"[";
        }
        if(model->getHandRender()){
            outHand.open(outputFile+"_hand.json");
            outHand<<"[";
        }
        if(model->getHandRender()){
            outHead.open(outputFile+"_head.json");
            outHead<<"[";
        }
        if(model->getRender3D()){
            outBody3d.open(outputFile+"_body3d.json");
            outBody3d<<"[";
        }
        if(model->getUseIk()){
            outIk.open(outputFile+"_ik.json");
            outIk<<"[";
        }
    }

    int framecount=0;

    while (1) {
        cap >> frame;
        if (frame.empty()) {
            std::cout << "empty frame\n";
            break;
        }
        try {
            poseEstimator.ProcessFrame(frame.data, frame.cols, frame.rows, poseOptions);
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what();
            return;
        }

        if(out.is_open()){
            wrnch::SerializedData localResultsToJson = poseEstimator.ResultsToJson();
            const char *localData = localResultsToJson.Data();
            out<<"{\"frame_time\":"<<framecount<<","<<"\"persons\":"<<localData<<"},";

            rapidjson::Document d;
            d.Parse(localData);

            auto a=d.GetArray();

            if(model->getBodyRender()){
                if(a[0].HasMember("pose2d")){
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    a[0]["pose2d"].Accept(writer);
                    outBody2d<<buffer.GetString()<<",";
                }
                else outBody2d<<"{},";
            }
            if(model->getHandRender()){
                if(a[0].HasMember("hand_pose")){
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    a[0]["hand_pose"].Accept(writer);
                    outHand<<buffer.GetString()<<",";
                }
                else outHand<<"{},";
            }
            if(model->getHeadRender()){
                if(a[0].HasMember("head_pose")){
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    a[0]["head_pose"].Accept(writer);
                    outHead<<buffer.GetString()<<",";
                }
                else outHead<<"{},";
            }
            if(model->getRender3D()){
                if(a[0].HasMember("pose_3d_raw")){
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    a[0]["pose_3d_raw"].Accept(writer);
                    outBody3d<<buffer.GetString()<<",";
                }
                else outBody3d<<"{},";
            }
            if(model->getUseIk()){
                if(a[0].HasMember("pose_3d_ik")){
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    a[0]["pose_3d_ik"].Accept(writer);
                    outIk<<buffer.GetString()<<",";
                }
                else outIk<<"{},";
            }
            framecount++;
        }

        visualizer.SetFrame(frame);

        if(model->getBodyRender()){
            if(model->getRender3D()||model->getUseIk()){
                cv::Mat backgroundFrame(frame.cols, frame.rows, CV_8UC3);
                backgroundFrame = cv::Scalar(255, 255, 255);
                visualizer.GetBillBoard(frame, backgroundFrame, 0, rotationY, 0);
                visualizer.SetFrame(backgroundFrame);
                if (model->getUseIk()) renderBodyIk(visualizer,bodyNumJoints,bodyNumBones,bodyBonePairs,rotationY);
                else if(model->getRender3D())renderBody3D(visualizer,bodyNumJoints,bodyNumBones,bodyBonePairs,rotationY);
            }else
                renderBody2D(visualizer,bodyNumJoints,bodyNumBones,bodyBonePairs);
        }
        if(model->getHeadRender()){
            renderHead(visualizer,facialNumJoints,facialNumBones,facialBonePairs);
        }
        if(model->getHandRender()){
            renderHand(visualizer,handNumJoints,handNumBones,handBonePairs);
        }

        char c = visualizer.WaitKeyboardInput(1);
        if (c == 27) break;

        if(model->getRender3D()||model->getUseIk()){
            visualizer.DrawTextInFrame("Rotate image about y axis clockwise(m), counter-clockwise(n)",0,visualizer.GetHeight()-5);
            if (c == 'm')
            {
                if (rotationY < 75)
                    rotationY += 15;
            }
            if (c == 'n')
            {
                if (rotationY > -75)
                    rotationY -= 15;
            }
        }

        if (model->getGreen_screen()) {
            if (c == 'g') model->setGreen_screen(!model->getGreen_screen());
            int maskHeight, maskWidth, maskDepth;
            poseEstimator.GetMaskDims(maskWidth, maskHeight, maskDepth);
            const unsigned char *maskData = poseEstimator.GetMaskView();
            visualizer.DrawMask(maskData, maskWidth, maskHeight);
            visualizer.DrawTextInFrame("Toggle greenscreen (g)", 0, visualizer.GetHeight());
        }
        visualizer.Show();
        if(video.isOpened()&&model->getVideo()) video.write(frame);
    }

    if(out.is_open()){
        out<<"]}";
        out.close();
    }
    if(outBody2d.is_open()) {outBody2d<<"{}]"; outBody2d.close();}
    if(outHand.is_open()) {outHand<<"{}]";outHand.close();}
    if(outHead.is_open()) {outHead<<"{}]";outHead.close();}
    if(outBody3d.is_open()) {outBody3d<<"{}]";outBody3d.close();}
    if(outIk.is_open()) {outIk<<"{}]";outIk.close();}

    cap.release();
    video.release();
    cv::destroyAllWindows();
    emit finished();
    emit updateStatus("Ready");
}

int Wrnch::start(){
    wrnchThread->start();
    return 0;
}

void Wrnch::DrawHands(wrnch::PoseEstimator::PoseHandsIter handsBegin, wrnch::PoseEstimator::PoseHandsIter handsEnd, Visualizer &visualizer, const unsigned int *bonePairs, const unsigned int handNumJoints, const unsigned int handNumBones)
{
    for(wrnch::PoseEstimator::PoseHandsIter it=handsBegin;it!=handsEnd;it++){
        wrnch::Pose2dView pose=*it;
        visualizer.DrawPoints(pose.GetJoints(),handNumJoints);
        visualizer.DrawLines(pose.GetJoints(),&bonePairs[0],handNumBones);
    }
}

void Wrnch::json2csv(QString fileName)
{
    auto jsonInput = make_shared<Json::Value>();
    QString input=fileName+".json";
    QString output=fileName+".csv";
    QByteArray inputbytearray=input.toLocal8Bit();
    QByteArray outputbytearray=output.toLocal8Bit();
    //check if input file exists
    auto ok = jsonToCsv(jsonInput, inputbytearray.data());
    if (!ok){
        qDebug()<<"Error when parse csv\n";
        exit(1);
    }

    //flatten the objects
    auto objects = jsonToDicts(jsonInput);

    //merge and sort the keys
    auto csv = dictsToCsv(objects);

    //export file
    ofstream ofs;
    ofs.open(outputbytearray.data());
    for (const auto&e : *csv){
        auto len = e.get()->size();
        int counter = 0;
        for (const auto&g : *e){
            ofs << *g << (counter < len - 1 ? "," : "");
            counter++;
        }
        ofs << "\n";
    }
    ofs.close();
}

void Wrnch::handleThreadFinished()
{
    if(model->getCsv()){
        QString fileName=model->getJson();
        if(model->getBodyRender())
            json2csv(fileName+"_body2d");
        if(model->getRender3D())
            json2csv(fileName+"_body3d");
        if(model->getHandRender())
            json2csv(fileName+"_hand");
        if(model->getHeadRender())
            json2csv(fileName+"_head");
        if(model->getUseIk())
            json2csv(fileName+"_ik");
    }
}

