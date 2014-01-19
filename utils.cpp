#include "utils.h"

QString utils::formatTime(const QString& format, const QTime& time)  // temporary global function, until better design ideas.
{
    QString formattedTime;

    if (format.endsWith("zzz"))
        formattedTime = time.toString(format);

    else if (format.endsWith("zz"))
    {
        QString original = time.toString(format + "z");
        formattedTime = original.left(original.size() - 1);
    }

    else if (format.endsWith("z"))
    {
        QString original = time.toString(format + "zz");
        formattedTime = original.left(original.size() - 2);
    }

    else
    {
        formattedTime = time.toString(format);
    }

    return formattedTime;
}
