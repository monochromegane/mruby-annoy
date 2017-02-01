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
#include "mruby/array.h"
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

static mrb_value mrb_annoy_index_add_item(mrb_state *mrb, mrb_value self)
{
  int item;
  mrb_value ary;
  mrb_get_args(mrb, "iA", &item, &ary);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  int len = RARRAY_LEN(ary);
  double *vec = (double *) malloc(len * sizeof(double));
  for (int i = 0; i < len; ++i){
    vec[i] = mrb_float(mrb_ary_ref(mrb, ary, i));
  }

  annoy_index->add_item(item, vec);
  return self;
}

static mrb_value mrb_annoy_index_load(mrb_state *mrb, mrb_value self)
{
  char *filename;
  int len;
  mrb_get_args(mrb, "s", &filename, &len);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  return mrb_bool_value(annoy_index->load(filename));
}

static mrb_value mrb_annoy_index_get_n_items(mrb_state *mrb, mrb_value self)
{
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  return mrb_fixnum_value(annoy_index->get_n_items());
}
static mrb_value mrb_annoy_index_get_nns_by_item(mrb_state *mrb, mrb_value self)
{
  int i;
  int n;
  int search_k = -1;
  bool include_distances = false;
  mrb_get_args(mrb, "ii|ib", &i, &n, &search_k, &include_distances);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));

  std::vector<int> closest;
  std::vector<double> distances;
  annoy_index->get_nns_by_item(i, n, search_k, &closest, include_distances ? &distances : NULL);

  mrb_value result = mrb_ary_new(mrb);
  for(double r : closest) {
    mrb_ary_push(mrb, result, mrb_float_value(mrb, r));
  }
  return result;
}

void mrb_mruby_annoy_gem_init(mrb_state *mrb)
{
  struct RClass *annoy_index = mrb_define_class(mrb, "AnnoyIndex", mrb->object_class);
  MRB_SET_INSTANCE_TT(annoy_index, MRB_TT_DATA);
  mrb_define_method(mrb, annoy_index, "initialize", mrb_annoy_index_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "add_item", mrb_annoy_index_add_item, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, annoy_index, "load", mrb_annoy_index_load, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "get_n_items", mrb_annoy_index_get_n_items, MRB_ARGS_NONE());
  mrb_define_method(mrb, annoy_index, "get_nns_by_item", mrb_annoy_index_get_nns_by_item, MRB_ARGS_ARG(2, 2));
  DONE;
}

void mrb_mruby_annoy_gem_final(mrb_state *mrb)
{
}

