#include "model.h"
#include <iostream>

Model* Model::m_instance=0;


Model::Model()
{
    type=Type::CAM;
    model_dir=nullptr;
    cam_index=-1;
    file="";
    bodyType="j25";
    headType="wrFace20";
    handType="hand21";
    bodyRender=true;
    headRender=false;
    handRender=false;
    activity=false;
    render3D=false;
    resolutionWidth=456;
    resolutionHeight=256;
    gpu=0;
    greenScreen=false;
    tracking=false;
    useIk=false;
    csv=false;
    video=false;
}

bool Model::getVideo() const
{
    return video;
}

void Model::setVideo(bool value)
{
    video = value;
}

bool Model::getCsv() const
{
    return csv;
}

void Model::setCsv(bool value)
{
    csv = value;
}

bool Model::getUseIk() const
{
    return useIk;
}

void Model::setUseIk(bool value)
{
    useIk = value;
}

QString Model::getMin_valid_joints() const
{
    return minValidJoints;
}

void Model::setMin_valid_joints(const QString &value)
{
    minValidJoints = value;
}

QString Model::getModel_regex() const
{
    return modelRegex;
}

void Model::setModel_regex(const QString &value)
{
    modelRegex = value;
}

void Model::setFbx(const QString &value)
{
    fbx = value;
}

QString Model::getInspect() const
{
    return inspect;
}

void Model::setInspect(const QString &value)
{
    inspect = value;
}

QString Model::getFbx() const
{
    return fbx;
}

int Model::getGpu() const
{
    return gpu;
}

void Model::setGpu(int value)
{
    gpu = value;
}

QString Model::getDeserialize() const
{
    return deserialize;
}

void Model::setDeserialize(const QString &value)
{
    deserialize = value;
}

QString Model::getSerialize() const
{
    return serialize;
}

void Model::setSerialize(const QString &value)
{
    serialize = value;
}

QString Model::getTracking_kind() const
{
    return trackingKind;
}

void Model::setTracking_kind(const QString &value)
{
    trackingKind = value;
}

bool Model::getTracking() const
{
    return tracking;
}

void Model::setTracking(bool value)
{
    tracking = value;
}

bool Model::getGreen_screen() const
{
    return greenScreen;
}

void Model::setGreen_screen(bool value)
{
    greenScreen = value;
}

QString Model::getOutmedia() const
{
    return outmedia;
}

void Model::setOutmedia(const QString &value)
{
    outmedia = value;
}

QString Model::getProto_video() const
{
    return protoVideo;
}

void Model::setProto_video(const QString &value)
{
    protoVideo = value;
}

QString Model::getInspect_proto() const
{
    return inspectProto;
}

void Model::setInspect_proto(const QString &value)
{
    inspectProto = value;
}

QString Model::getProto() const
{
    return proto;
}

void Model::setProto(const QString &value)
{
    proto = value;
}

QString Model::getJson() const
{
    return json;
}

void Model::setJson(const QString &value)
{
    json = value;
}

Model* Model::getInstance(){
    if(!m_instance)
        m_instance=new Model();
    return m_instance;
}

QString Model::getArgs()
{
    QString temp;
    temp.append("--input \""+file+"\"");
    if(!model_dir.isEmpty()) temp.append(" --models-dir \""+model_dir+"\"");
    if(!json.isEmpty()) temp.append(" --json \""+json+"\"");
    if(!proto.isEmpty()) temp.append(" --proto \""+proto+"\"");
    if(!inspectProto.isEmpty()) temp.append(" --inspect-proto \""+inspectProto+"\"");
    if(!protoVideo.isEmpty()) temp.append(" --proto-video \""+protoVideo+"\"");
    if(!outmedia.isEmpty()) temp.append(" --outmedia \""+outmedia+"\"");
    if(greenScreen) temp.append(" --green-screen ");
    if(headRender) temp.append(" --head-pose ");
    if(handRender) temp.append(" --hand-pose ");
    if(tracking) temp.append(" --tracking ");
    if(!trackingKind.isEmpty()) temp.append(" --tracker-kind "+trackingKind);
    if(render3D) temp.append(" --3d-pose ");
    if(useIk) temp.append(" --use-ik ");
    if(!deserialize.isEmpty()) temp.append(" --deserialize \""+deserialize+"\"");
    if(!serialize.isEmpty()) temp.append(" --serialize \""+serialize+"\"");
    if(gpu!=0) temp.append(" --gpu "+QString::number(gpu));
    if(resolutionWidth&resolutionHeight) temp.append(" --net-res "+QString::number(resolutionWidth)+"x"+QString::number(resolutionHeight));
    if(!fbx.isEmpty()) temp.append(" --fbx \""+fbx+"\"");
    if(!inspect.isEmpty()) temp.append("- -inspect "+inspect);
    if(!modelRegex.isEmpty()) temp.append(" --2d-model-regex "+modelRegex);
    if(!minValidJoints.isEmpty()) temp.append(" --min-valid-joints "+minValidJoints);
    return temp;
}

//setters and getters

QString Model::getModel_dir() const
{
    return model_dir;
}

void Model::setModel_dir(const QString& value)
{
    model_dir = value;
}

QString Model::getFile() const
{
    return file;
}

void Model::setFile(const QString& value)
{
    file=value;
}

int Model::getCam_index() const
{
    return cam_index;
}

void Model::setCam_index(int value)
{
    cam_index = value;
}

Model::Type Model::getType() const
{
    return type;
}

void Model::setType(const Model::Type &value)
{
    type = value;
}

bool Model::getBodyRender() const
{
    return bodyRender;
}

void Model::setBodyRender(bool value)
{
    bodyRender = value;
}

QString Model::getBodyType() const
{
    return bodyType;
}

void Model::setBodyType(const QString &value)
{
    bodyType = value;
}

bool Model::getHeadRender() const
{
    return headRender;
}

void Model::setHeadRender(bool value)
{
    headRender = value;
}

QString Model::getHeadType() const
{
    return headType;
}

void Model::setHeadType(const QString &value)
{
    headType = value;
}

bool Model::getHandRender() const
{
    return handRender;
}

void Model::setHandRender(bool value)
{
    handRender = value;
}

QString Model::getHandType() const
{
    return handType;
}

void Model::setHandType(const QString &value)
{
    handType = value;
}

int Model::getResolution_width() const
{
    return resolutionWidth;
}

void Model::setResolution_width(int value)
{
    resolutionWidth = value;
}

int Model::getResolution_height() const
{
    return resolutionHeight;
}

void Model::setResolution_height(int value)
{
    resolutionHeight = value;
}

bool Model::getRender3D() const
{
    return render3D;
}

void Model::setRender3D(bool value)
{
    render3D = value;
}

bool Model::getActivity() const
{
    return activity;
}

void Model::setActivity(bool value)
{
    activity = value;
}

