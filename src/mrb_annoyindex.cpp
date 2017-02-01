/*
** mrb_annoyindex.cpp - AnnoyIndex class
**
** Copyright (c) monochromegane 2017
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_annoyindex.h"
#include "annoylib.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

void mrb_mruby_annoy_gem_init(mrb_state *mrb)
{
  DONE;
}

void mrb_mruby_annoy_gem_final(mrb_state *mrb)
{
}

