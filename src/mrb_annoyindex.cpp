/*
** mrb_annoyindex.cpp - AnnoyIndex class
**
** Copyright (c) monochromegane 2017
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include <mruby/class.h>
#include "mruby/data.h"
#include "mrb_annoyindex.h"
#include "annoylib.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static void annoy_index_free(mrb_state *mrb, void *ptr)
{
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(ptr);
  delete annoy_index;
}

static struct mrb_data_type annoy_index_type = { "AnnoyIndex", annoy_index_free };

static mrb_value mrb_annoy_index_init(mrb_state *mrb, mrb_value self)
{
  DATA_TYPE(self) = &annoy_index_type;
  DATA_PTR(self) = NULL;

  mrb_int f;
  mrb_get_args(mrb, "i", &f);
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = new AnnoyIndex<int, double, Angular, RandRandom>(f);

  DATA_PTR(self) = annoy_index;
  return self;
}

void mrb_mruby_annoy_gem_init(mrb_state *mrb)
{
  struct RClass *annoy_index = mrb_define_class(mrb, "AnnoyIndex", mrb->object_class);
  MRB_SET_INSTANCE_TT(annoy_index, MRB_TT_DATA);
  mrb_define_method(mrb, annoy_index, "initialize", mrb_annoy_index_init, MRB_ARGS_REQ(1));
  DONE;
}

void mrb_mruby_annoy_gem_final(mrb_state *mrb)
{
}

