#ifndef ANDROIDOUT_H
#define ANDROIDOUT_H

#include <android/log.h>
#include <sstream>

namespace Solar {

    extern std::ostream out;

    class AndroidOut : public std::stringbuf {
    public:
        inline AndroidOut(const char *kLogTag) : logTag(kLogTag)
        {
        }

    protected:
        virtual int sync() override
        {
            __android_log_print(ANDROID_LOG_DEBUG, logTag, "%s", str().c_str());
            str("");
            return 0;
        }

    private:
        const char *logTag;
    };

}

#endif