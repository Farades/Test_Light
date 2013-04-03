#-------------------------------------------------
#
# Project created by QtCreator 2013-04-03T13:03:59
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

LIBS     +=-lglut32 -lglu32 -lopengl32 -lwinmm -lgdi32 -ldevil
LIBS     +=-lilu  -lilut

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Test_Light
TEMPLATE = app


SOURCES += main.cpp

HEADERS  +=
