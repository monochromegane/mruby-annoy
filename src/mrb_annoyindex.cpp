/*
** mrb_annoyindex.cpp - AnnoyIndex class
**
** Copyright (c) monochromegane 2017
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/class.h"
#include "mruby/data.h"
#include "mruby/variable.h"
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

  mrb_value f;
  mrb_get_args(mrb, "i", &f);
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = new AnnoyIndex<int, double, Angular, RandRandom>(mrb_fixnum(f));

  DATA_PTR(self) = annoy_index;
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@f"), f);
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

static mrb_value mrb_annoy_index_build(mrb_state *mrb, mrb_value self)
{
  int n_trees;
  mrb_get_args(mrb, "i", &n_trees);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  annoy_index->build(n_trees);
  return self;
}

static mrb_value mrb_annoy_index_save(mrb_state *mrb, mrb_value self)
{
  char *filename;
  int len;
  mrb_get_args(mrb, "s", &filename, &len);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  return mrb_bool_value(annoy_index->save(filename));
}

static mrb_value mrb_annoy_index_load(mrb_state *mrb, mrb_value self)
{
  char *filename;
  int len;
  mrb_get_args(mrb, "s", &filename, &len);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  return mrb_bool_value(annoy_index->load(filename));
}

static mrb_value mrb_annoy_index_unload(mrb_state *mrb, mrb_value self)
{
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  annoy_index->unload();
  return self;
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

static mrb_value mrb_annoy_index_get_nns_by_vector(mrb_state *mrb, mrb_value self)
{
  mrb_value ary;
  int n;
  int search_k = -1;
  bool include_distances = false;
  mrb_get_args(mrb, "Ai|ib", &ary, &n, &search_k, &include_distances);

  int len = RARRAY_LEN(ary);
  double *vec = (double *) malloc(len * sizeof(double));
  for (int i = 0; i < len; ++i){
    vec[i] = mrb_float(mrb_ary_ref(mrb, ary, i));
  }

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));

  std::vector<int> closest;
  std::vector<double> distances;
  annoy_index->get_nns_by_vector(&vec[0], n, search_k, &closest, include_distances ? &distances : NULL);

  mrb_value result = mrb_ary_new(mrb);
  for(double r : closest) {
    mrb_ary_push(mrb, result, mrb_float_value(mrb, r));
  }
  return result;
}

static mrb_value mrb_annoy_index_get_item_vector(mrb_state *mrb, mrb_value self)
{
  int i;
  mrb_get_args(mrb, "i", &i);

  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));

  mrb_value f = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@f"));
  std::vector<double> vec(mrb_fixnum(f));
  annoy_index->get_item(i, &vec[0]);

  mrb_value result = mrb_ary_new(mrb);
  for(double v : vec) {
    mrb_ary_push(mrb, result, mrb_float_value(mrb, v));
  }
  return result;
}

static mrb_value mrb_annoy_index_get_n_items(mrb_state *mrb, mrb_value self)
{
  AnnoyIndex<int, double, Angular, RandRandom>* annoy_index = static_cast<AnnoyIndex<int, double, Angular, RandRandom>*>(mrb_get_datatype(mrb, self, &annoy_index_type));
  return mrb_fixnum_value(annoy_index->get_n_items());
}

void mrb_mruby_annoy_gem_init(mrb_state *mrb)
{
  struct RClass *annoy_index = mrb_define_class(mrb, "AnnoyIndex", mrb->object_class);
  MRB_SET_INSTANCE_TT(annoy_index, MRB_TT_DATA);
  mrb_define_method(mrb, annoy_index, "initialize", mrb_annoy_index_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "add_item", mrb_annoy_index_add_item, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, annoy_index, "build", mrb_annoy_index_build, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "save", mrb_annoy_index_save, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "load", mrb_annoy_index_load, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "unload", mrb_annoy_index_unload, MRB_ARGS_NONE());
  mrb_define_method(mrb, annoy_index, "get_nns_by_item", mrb_annoy_index_get_nns_by_item, MRB_ARGS_ARG(2, 2));
  mrb_define_method(mrb, annoy_index, "get_nns_by_vector", mrb_annoy_index_get_nns_by_vector, MRB_ARGS_ARG(2, 2));
  mrb_define_method(mrb, annoy_index, "get_item_vector", mrb_annoy_index_get_item_vector, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, annoy_index, "get_n_items", mrb_annoy_index_get_n_items, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_annoy_gem_final(mrb_state *mrb)
{
}

