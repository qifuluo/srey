#!/bin/sh
#***********************************************
# File Name  : mk
# Description: make file
# Make By    :
# Date Time  :2011/06/15 
#***********************************************

UsAge="UsAge:\"./mk.sh\" or \"./mk.sh x86/x64\" \"./mk.sh clean\""

#���ɳ��������
PROGRAMNAME="srey"
#�ļ���
Dir="lib lib/event lib/md5 lib/sha1 test"
#SSL��
SSLLIB="-lssl -lcrypto"
#main���������ļ���
MAINDIR="test"
#main���������ļ�
MAINFILE="main.c"
#���Ӱ�����
INCLUDELIB="-lpthread -lm"
#ϵͳ λ��
OSNAME=`uname`
if [ "$OSNAME" != "Darwin" ]
then
    INCLUDELIB=$INCLUDELIB" -lrt"
fi
if [ "$OSNAME" = "Linux" ]
then
    INCLUDELIB=$INCLUDELIB" -ldl"
fi
if [ "$OSNAME" = "SunOS" ]
then
	INCLUDELIB=$INCLUDELIB" -lsocket -lnsl"
fi
X64=""
OS_Version=`uname -m`
echo $OSNAME $OS_Version
if [ "$OS_Version" = "x86_64" ] || [ "$OS_Version" = "amd64" ]
then
	X64="x64"
fi
if [ $# -eq 1 ]
then
    if [ "$1" = "x64" ]
    then
        X64="x64"
    fi
	if [ "$1" = "x86" ]
    then
        X64=""
    fi
fi
#������·��
RSTPATH="bin"
#�м���ļ���
LIBNAME="srey"
#�����������ļ�,MAINFILE+����
EXCEPTL=$MAINFILE" "$TESTFILE
MAKEFILEPATH=`pwd`
LIBPATH="-L$MAKEFILEPATH/$RSTPATH"
CC="gcc -std=gnu99"
GCC="g++"
ARCH="ar -rv"
INCLUDEPATH=""
OBJFILE=""
CFLAGS="-Wall -O2"
if [ "$X64" = "x64" ]
then
    CFLAGS=$CFLAGS" -m64"
fi
LIBDIR=$Dir
Clean()
{
    rm -rf *.o
    for EachSub in $LIBDIR
    do
	    rm -rf $MAKEFILEPATH/$EachSub/*.o
    done

    cd $RSTPATH    
    rm -rf lib$LIBNAME.a
    rm -rf $PROGRAMNAME
    cd $MAKEFILEPATH
}
Clean
GetIncludePath()
{
    for EachSub in $LIBDIR
    do
        INCLUDEPATH=$INCLUDEPATH" -I$MAKEFILEPATH/$EachSub"
    done
    
    echo ---------------------Dir---------------------------
    echo $LIBDIR

    echo ---------------------Include Dir---------------------------
    echo $INCLUDEPATH
}

IsExcePTL()
{
    for EachExec in $EXCEPTL
    do
        if [ "$EachExec" = "$1" ]
        then
            return 1
        fi
    done

    return 0
}

Make()
{
    for EachSub in $LIBDIR
    do
        echo ---------------------$EachSub--------------------------- 
        cd $EachSub
        if [ "$?" != "0" ]
        then
            exit 1
        fi
		
		SourceFile=`ls *.cpp 2>/dev/null`
        for EachFile in $SourceFile
        do
            IsExcePTL $EachFile
            if [ "$?" = "0" ]
            then
                echo "$GCC $CFLAGS -c $EachFile"
                $GCC $CFLAGS -c $EachFile $INCLUDEPATH
                if [ "$?" != "0" ]
                then
                    echo "---------------------Error---------------------"
                    exit 1
                fi
            fi
        done

        SourceFile=`ls *.c 2>/dev/null`
        for EachFile in $SourceFile
        do
            IsExcePTL $EachFile
            if [ "$?" = "0" ]
            then
                echo "$CC $CFLAGS -c $EachFile"
                $CC $CFLAGS -c $EachFile $INCLUDEPATH
                if [ "$?" != "0" ]
                then
                    echo "---------------------Error---------------------"
                    exit 1
                fi
            fi
        done
        
	RstFile=`ls *.o 2>/dev/null`
	for EachFile in $RstFile
        do
            if [ -f "$MAKEFILEPATH/$EachFile" ]
            then
                rm -rf $MAKEFILEPATH/$EachFile
            fi
            mv $EachFile $MAKEFILEPATH>/dev/null
        done
        cd $MAKEFILEPATH
    done

    echo ---------------------Make .a file--------------------------- 
    cd $MAKEFILEPATH
    OBJFILE=`ls *.o 2>/dev/null`
    echo "$ARCH lib$LIBNAME.a `echo $OBJFILE`"
    $ARCH lib$LIBNAME.a `echo $OBJFILE`
    rm -rf *.o
    mv lib$LIBNAME.a $RSTPATH
}

while [ 1 = 1 ]
do
    if [ $# -eq 1 ]
    then
        if [ "$1" = "clean" ]
        then
            Clean
            exit 0
        fi
		if [ "$1" = "x64" ]
        then
            break
        fi
		if [ "$1" = "x86" ]
        then
            break
        fi
        echo "$UsAge"
        exit 1
    elif [ $# -gt 1 ]
    then
        echo "$UsAge"
        exit 1
    else
        break
    fi
done

GetIncludePath
Make

mkmaindir=$MAKEFILEPATH/$MAINDIR
mkmaincpp=$MAINFILE
proname=$PROGRAMNAME

cd $mkmaindir

echo ---------------------Make program file---------------------------
echo "$CC $CFLAGS $mkmaincpp $LIBAPP $INCLUDEPATH $LIBPATH $INCLUDELIB -l$LIBNAME -o $proname $SSLLIB"
$CC $CFLAGS $mkmaincpp $LIBAPP $INCLUDEPATH $LIBPATH $INCLUDELIB -l$LIBNAME -o $proname $SSLLIB
if [ "$?" != "0" ]
then
    echo "---------------------Error---------------------"
    exit 1
fi

mv $proname $MAKEFILEPATH/$RSTPATH
cd $MAKEFILEPATH

exit 0
