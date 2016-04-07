#-----------------------------------------------------------------------------
# GNU Makefile for static libraries: project dependent part
#
# $Id: Makefile.prj.inc,v 1.2 2006/10/20 20:20:15 emarkus Exp $
# $Source: /usr/cvsroot/project_hog/build/gmake/apps/nathan/Makefile.prj.inc,v $
#-----------------------------------------------------------------------------

NAME = asg1test
DBG_NAME = $(NAME)
REL_NAME = $(NAME)

ROOT = ../../..
VPATH = $(ROOT)

DBG_OBJDIR = $(ROOT)/objs/$(NAME)/debug
REL_OBJDIR = $(ROOT)/objs/$(NAME)/release
DBG_BINDIR = $(ROOT)/bin/debug
REL_BINDIR = $(ROOT)/bin/release

PROJ_CXXFLAGS = -I$(ROOT)/algorithms -I$(ROOT)/environments 

PROJ_DBG_CXXFLAGS = $(PROJ_CXXFLAGS)
PROJ_REL_CXXFLAGS = $(PROJ_CXXFLAGS)

PROJ_DBG_LNFLAGS = -L$(DBG_BINDIR)
PROJ_REL_LNFLAGS = -L$(REL_BINDIR)

PROJ_DBG_LIB =  -lenvironments
PROJ_REL_LIB =  -lenvironments


PROJ_DBG_DEP = \
  $(DBG_BINDIR)/libenvironments.a \

PROJ_REL_DEP = \
  $(REL_BINDIR)/libenvironments.a \



default : all

SRC_CPP = \
	apps/asg1test/Test.cpp \


