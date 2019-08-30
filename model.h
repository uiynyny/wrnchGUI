#ifndef MODEL_H
#define MODEL_H
#include <QString>



class Model {
public:
    enum Type {CAM,FILE};

    static Model* getInstance();

    QString getArgs();

    QString getModel_dir() const;
    void setModel_dir(const QString&);

    QString getFile() const;
    void setFile(const QString&);

    int getCam_index() const;
    void setCam_index(int);

    Model::Type getType() const;
    void setType(const Model::Type &value);

    bool getBodyRender() const;
    void setBodyRender(bool value);

    QString getBodyType() const;
    void setBodyType(const QString &value);

    bool getHeadRender() const;
    void setHeadRender(bool value);

    QString getHeadType() const;
    void setHeadType(const QString &value);

    bool getHandRender() const;
    void setHandRender(bool value);

    QString getHandType() const;
    void setHandType(const QString &value);

    int getResolution_width() const;
    void setResolution_width(int value);

    int getResolution_height() const;
    void setResolution_height(int value);

    bool getRender3D() const;
    void setRender3D(bool value);

    bool getActivity() const;
    void setActivity(bool value);

    QString getJson() const;
    void setJson(const QString &value);

    QString getProto() const;
    void setProto(const QString &value);

    QString getInspect_proto() const;
    void setInspect_proto(const QString &value);

    QString getProto_video() const;
    void setProto_video(const QString &value);

    QString getOutmedia() const;
    void setOutmedia(const QString &value);

    bool getGreen_screen() const;
    void setGreen_screen(bool value);

    bool getTracking() const;
    void setTracking(bool value);

    QString getTracking_kind() const;
    void setTracking_kind(const QString &value);

    QString getSerialize() const;
    void setSerialize(const QString &value);

    QString getDeserialize() const;
    void setDeserialize(const QString &value);

    int getGpu() const;
    void setGpu(int value);

    QString getFbx() const;

    QString getInspect() const;
    void setInspect(const QString &value);

    void setFbx(const QString &value);

    QString getModel_regex() const;
    void setModel_regex(const QString &value);

    QString getMin_valid_joints() const;
    void setMin_valid_joints(const QString &value);

    bool getUseIk() const;
    void setUseIk(bool value);

    bool getCsv() const;
    void setCsv(bool value);

    bool getVideo() const;
    void setVideo(bool value);

private:
    static Model* m_instance;
    Model();
    Model::Type type;
    QString model_dir;
    QString file;
    int cam_index;
    bool bodyRender;
    QString bodyType;
    bool headRender;
    QString headType;
    bool handRender;
    QString handType;
    bool render3D;
    bool activity;
    int resolutionWidth;
    int resolutionHeight;

    QString json;
    QString proto;
    QString inspectProto;
    QString protoVideo;
    QString outmedia;
    bool greenScreen;
    bool tracking;
    QString trackingKind;
    QString serialize;
    QString deserialize;
    int gpu;
    QString fbx;
    QString inspect;
    QString modelRegex;
    QString minValidJoints;
    bool useIk;
    bool csv;
    bool video;

};

#endif // MODEL_H
