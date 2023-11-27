#include "resource.h"

// return body picture path
QString Body::getBody(Body::BodyName index)
{
    switch (index)
    {
    case Body:
        return ":/assets/appearance/body/def_body.png";
        break;
    case BlueBody:
        return ":/assets/appearance/body/blue_body.png";
        break;
    case PinkBody:
        return ":/assets/appearance/body/pink_body.png";
        break;
    case IceFireBody:
        return ":/assets/appearance/body/icefire_body.png";
        break;
    case CatBody:
        return ":/assets/appearance/body/cat_body.png";
        break;
    case GundamBody:
        return ":/assets/appearance/body/Gundam_body.png";
        break;
    case DrillBody:
        return ":/assets/appearance/body/drill_body.png";
        break;
    case AngelBody:
        return ":/assets/appearance/body/angel_body.png";
        break;
    case Dummy:
        return ":/assets/appearance/body/dummy.png";
        break;
    default:
        throw "Invalid index";
        break;
    }
}

// return ear picture path
QString Ear::getEar(Ear::EarName index)
{
    switch (index)
    {
    case Ear:
        return ":/assets/appearance/ears/def_ears1.png";
        break;
    case BlueEar:
        return ":/assets/appearance/ears/blue_ears1.png";
        break;
    case PinkEar:
        return ":/assets/appearance/ears/pink_ears1.png";
        break;
    case IceFireEar:
        return ":/assets/appearance/ears/icefire_ears1.png";
        break;
    case CatEar:
        return ":/assets/appearance/ears/cat_ears1.png";
        break;
    case GundamEar:
        return ":/assets/appearance/ears/Gundam_ears1.png";
        break;
    case DrillEar:
        return ":/assets/appearance/ears/drill_ears1.png";
        break;
    case AngelEar:
        return ":/assets/appearance/ears/angel_ears1.png";
        break;
    default:
        throw "Invalid index";
        break;
    }
}

QString Ear::getEar2(Ear::EarName2 index)
{
    switch (index)
    {
    case Ear2:
        return ":/assets/appearance/ears/def_ears2.png";
        break;
    case BlueEar2:
        return ":/assets/appearance/ears/blue_ears2.png";
        break;
    case PinkEar2:
        return ":/assets/appearance/ears/pink_ears2.png";
        break;
    case IceFireEar2:
        return ":/assets/appearance/ears/icefire_ears2.png";
        break;
    case CatEar2:
        return ":/assets/appearance/ears/cat_ears2.png";
        break;
    case GundamEar2:
        return ":/assets/appearance/ears/Gundam_ears2.png";
        break;
    case DrillEar2:
        return ":/assets/appearance/ears/drill_ears2.png";
        break;
    case AngelEar2:
        return ":/assets/appearance/ears/angel_ears2.png";
        break;
    default:
        throw "Invalid index";
        break;
    }
}

// return eye picture path
QString Eye::getEye(Eye::EyeName index)
{
    switch (index)
    {
    case Eye:
        return ":/assets/appearance/eyes/def_eyes.png";
        break;

    default:
        throw "Invalid index";
        break;
    }
}

// return stripe picture path
QString Stripe::getStripe(Stripe::StripeName index)
{
    switch (index)
    {
    case Stripe:
        return ":/assets/appearance/stripe.png";
        break;

    default:
        throw "Invalid index";
        break;
    }
}

// return icon picture path
QString HaroIcon::getIcon(HaroIcon::IconName index)
{
    switch (index)
    {
    case Cabinet:
        return ":/assets/icons/cabinet.png";
        break;
    case Calendar:
        return ":/assets/icons/calendar.png";
        break;
    case Close:
        return ":/assets/icons/close.png";
        break;
    case Dress:
        return ":/assets/icons/dress.png";
        break;
    case Icon:
        return ":/assets/icons/haro_icon.ico";
        break;
    case Management:
        return ":/assets/icons/management.png";
        break;
    case Min:
        return ":/assets/icons/min.png";
        break;
    case More:
        return ":/assets/icons/more.png";
        break;
    case Setting:
        return ":/assets/icons/setting.png";
        break;
    case Star:
        return ":/assets/icons/star.png";
        break;
    case Work:
        return ":/assets/icons/work.png";
        break;
    case choose:
        return ":/assets/icons/choose.png";
        break;
    default:
        throw "Invalid index";
        break;
    }
}

int Movement::getMovementNum(Movement::MovementKind index)
{
    switch (index)
    {
    case Blink:
        return 17;
        break;
    case CloseEyes:
        return 29;
    case Cruel:
        return 1;
    case Error:
        return 11;
    case Fly:
        return 43;
    case Hand:
        return 1;
    case Heart:
        return 23;
    case Question:
        return 31;
    case Wink:
        return 17;
    default:
        throw "Invalid index";
        break;
    }
}

QString Movement::getMovementName(Movement::MovementKind index)
{
    switch (index)
    {
    case Blink:
        return "blink";
        break;
    case CloseEyes:
        return "closeEyes";
    case Cruel:
        return "cruel";
    case Error:
        return "error";
    case Fly:
        return "fly";
    case Hand:
        return "hand";
    case Heart:
        return "heart";
    case Question:
        return "question";
    case Wink:
        return "wink";
    default:
        throw "Invalid index";
        break;
    }
}
