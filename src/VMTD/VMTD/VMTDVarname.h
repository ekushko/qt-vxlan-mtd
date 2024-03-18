#pragma once

#define VN(Variable) (#Variable)
#define VN_S(Variable) QString(#Variable)
#define VN_UP(Variable) QString(#Variable).replace(0, 1, QString(#Variable).at(0).toLower())
#define VN_ME(Variable) (QString(#Variable).remove("m_"))
#define VN_NS(Variable) (QString(#Variable).split("::")[1])
#define VN_NS_2(Variable) (QString(#Variable).split("::")[2])
#define VN_MT(Variable) (QString(#Variable).left(QString(#Variable).indexOf('(')))
#define LIO(V,C) QString(#V).lastIndexOf(C)
#define VN_MT_PTR(V) (VN_S(V).mid(LIO(V,">") + 1, LIO(V,"(") - (LIO(V,">") + 1)))
#define VN_MT_REF(V) (VN_S(V).mid(LIO(V,".") + 1, LIO(V,"(") - (LIO(V,".") + 1)))
