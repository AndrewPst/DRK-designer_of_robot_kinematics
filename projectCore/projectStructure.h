#ifndef PROJECTSTRUCTURE_H
#define PROJECTSTRUCTURE_H

#include <QVector3D>
#include <QList>
#include <QObject>


//-------Joint structure-------------

//Type of joint
enum class JointType_t
{
    JOINT_UNKNOWN = 0,
    JOINT_ROTATION,
    JOINT_LINEAR
};

//Denavit–Hartenberg parameters
struct DHParameters_t
{
    float d, o, r, a;

    bool operator==(const DHParameters_t& p){
        return d == p.d && o == p.o && r == p.r && a == p.a;
    }
};

struct Joint_t : public QObject
{
    Q_OBJECT

    Q_PROPERTY(JointType_t type READ getJointType WRITE setJointType NOTIFY onTypeChanged)
    Q_PROPERTY(QVector3D position READ getPosition WRITE setPosition NOTIFY onPositionChanged)
    Q_PROPERTY(QVector3D rotation READ getRotation WRITE setRotation NOTIFY onRotationChanged)
    Q_PROPERTY(DHParameters_t dhParams READ getDHParams WRITE setDHParams NOTIFY onDHParamsChanged)
    Q_PROPERTY(double minValue READ getMinValue WRITE setMinValue NOTIFY onMinValueChanged)
    Q_PROPERTY(double maxValue READ getMaxValue WRITE setMaxValue NOTIFY onMaxValueChanged)
    Q_PROPERTY(double currentValue READ getCurrentValue WRITE setCurrentValue NOTIFY onCurrentValueChanged)

    friend class ProjectController;

public:
    explicit Joint_t();
    ~Joint_t();

    JointType_t getJointType() const;
    void setJointType(const JointType_t&);

    QVector3D getPosition() const;
    void setPosition(const QVector3D&);

    QVector3D getRotation() const;
    void setRotation(const QVector3D&);

    DHParameters_t getDHParams() const;
    void setDHParams(const DHParameters_t&);

    double getMinValue() const;
    void setMinValue(double);

    double getMaxValue() const;
    void setMaxValue(double);

    double getCurrentValue() const;
    void setCurrentValue(double);

signals:

    void onTypeChanged(JointType_t);
    void onIdChanged(int);
    void onParentIdChanged(int);
    void onPositionChanged(QVector3D);
    void onRotationChanged(QVector3D);
    void onDHParamsChanged(DHParameters_t);
    void onMinValueChanged(double);
    void onMaxValueChanged(double);
    void onCurrentValueChanged(double);


private:

    JointType_t _type;
    QVector3D _position; //position relative to the parent object
    QVector3D _rotation; //rotation relative to the parent object
    DHParameters_t _dhParameters;
    double _minValue, _maxValue;

    //Non file parameters
    double _currentValue;
};

//----------Manipulator structure-----------

enum class KinematicsType_t
{
    KINEMATICS_UNKNOWN = 0,
    KINEMATICS_DEFAULT
};

struct Manipulator_t
{
    int dof;
    KinematicsType_t kinematicsType;
    QList<Joint_t*> joints;
};


//---------Project structure-----------

struct ProjectStructure
{
    QString projectName;
    QString projectVersion;
    Manipulator_t manipulator;
};

#endif // PROJECTSTRUCTURE_H
