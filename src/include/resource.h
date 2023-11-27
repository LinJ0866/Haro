#ifndef RESOURCES_H
#define RESOURCES_H

#include <QString>

using namespace std;

namespace Body
{
    // chose body picture by enum
    enum BodyName
    {
        Body = 0,
        BlueBody,
        PinkBody,
        IceFireBody,
        CatBody,
        GundamBody,
        DrillBody,
        AngelBody,
        Dummy = 20
    };

    const int BodyNum = 8;

    QString getBody(BodyName index);
}

namespace Ear
{
    // chose ear picture by enum
    enum EarName
    {
        Ear = 0,
        BlueEar,
        PinkEar,
        IceFireEar,
        CatEar,
        GundamEar,
        DrillEar,
        AngelEar
    };

    // store ear animation in two enum so we can get picture path with same index
    enum EarName2
    {
        Ear2,
        BlueEar2,
        PinkEar2,
        IceFireEar2,
        CatEar2,
        GundamEar2,
        DrillEar2,
        AngelEar2
    };

    // Ear number except Ear followed by 2
    const int EarNum = 8;

    QString getEar(EarName index);

    QString getEar2(EarName2 index);
}

namespace Eye
{
    // chose eye picture by enum
    enum EyeName
    {
        Eye = 0
    };

    QString getEye(EyeName index);
} // namespace Eye

namespace Stripe
{
    // chose stripe picture by enum
    enum StripeName
    {
        Stripe = 0
    };

    QString getStripe(StripeName index);
} // namespace Stripe

namespace HaroIcon
{
    // chose icon by enum
    enum IconName
    {
        Cabinet,
        Calendar,
        Close,
        Dress,
        Icon,
        Management,
        Min,
        More,
        Setting,
        Star,
        Work,
        choose
    };

    QString getIcon(IconName index);
}

namespace Movement
{
    enum MovementKind
    {
        CloseEyes=0,
        Heart=1,
        Question=2,
        Wink=3,
        Cruel=4,
        Blink = 10,
        Error = 11,
        Fly=12,
        Hand=13
    };
    const int movementNum = 5;
    // get picture number of specific movement
    int getMovementNum(MovementKind index);
    // get path name of specific movement
    QString getMovementName(MovementKind index);
} // namespace Movement

#endif
