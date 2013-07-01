/***************************************************************************
 *            ncm_fit_nlopt.c
 *
 *  Sat Apr  3 16:07:02 2010
 *  Copyright  2010  Sandro Dias Pinto Vitenti
 *  <sandro@isoftware.com.br>
 ****************************************************************************/
/*
 * numcosmo
 * Copyright (C) 2012 Sandro Dias Pinto Vitenti <sandro@isoftware.com.br>
 * 
 * numcosmo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * numcosmo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION:ncm_fit_nlopt
 * @title: NLopt Interface Object
 * @short_description: Interface for NLopt optmization library
 *
 * FIXME
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif /* HAVE_CONFIG_H */
#include "build_cfg.h"

#ifdef NUMCOSMO_HAVE_NLOPT

#include "math/ncm_fit_nlopt.h"
#include "math/ncm_cfg.h"
#include "ncm_fit_nlopt_enum.h"

enum
{
  PROP_0,
  PROP_ALGO,
  PROP_LOCAL_ALGO,
  PROP_SIZE,
};

G_DEFINE_TYPE (NcmFitNLOpt, ncm_fit_nlopt, NCM_TYPE_FIT);

static void
ncm_fit_nlopt_init (NcmFitNLOpt *fit_nlopt)
{
#ifdef HAVE_NLOPT_2_2
  fit_nlopt->nlopt = NULL;
  fit_nlopt->local_nlopt = NULL;
#endif /* HAVE_NLOPT_2_2 */
  fit_nlopt->nlopt_algo = 0;
  fit_nlopt->local_nlopt_algo = 0;
  fit_nlopt->desc = NULL;
}

static void
_ncm_fit_nlopt_constructed (GObject *object)
{
  /* Chain up : start */
  G_OBJECT_CLASS (ncm_fit_nlopt_parent_class)->constructed (object);
  {
    NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (object);
    NcmFit *fit = NCM_FIT (fit_nlopt);
    
    fit_nlopt->fparam_len = fit->fstate->fparam_len;

    fit_nlopt->lb = ncm_vector_new (fit_nlopt->fparam_len);
    fit_nlopt->ub = ncm_vector_new (fit_nlopt->fparam_len);
    fit_nlopt->pabs = ncm_vector_new (fit_nlopt->fparam_len);
    fit_nlopt->pscale = ncm_vector_new (fit_nlopt->fparam_len);
        
    ncm_fit_nlopt_set_algo (fit_nlopt, fit_nlopt->nlopt_algo);
  }
}

static void
_ncm_fit_nlopt_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (object);
  g_return_if_fail (NCM_IS_FIT_NLOPT (object));

  switch (prop_id)
  {
    case PROP_ALGO:
#ifdef HAVE_NLOPT_2_2
      if (fit_nlopt->nlopt == NULL)
        fit_nlopt->nlopt_algo = g_value_get_enum (value);
      else
        ncm_fit_nlopt_set_algo (fit_nlopt, g_value_get_enum (value));
#else
      ncm_fit_nlopt_set_algo (fit_nlopt, g_value_get_enum (value));
#endif /* HAVE_NLOPT_2_2 */
      break;
    case PROP_LOCAL_ALGO:
      ncm_fit_nlopt_set_local_algo (fit_nlopt, g_value_get_enum (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
_ncm_fit_nlopt_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (object);
  g_return_if_fail (NCM_IS_FIT_NLOPT (object));

  switch (prop_id)
  {
    case PROP_ALGO:
      g_value_set_enum (value, fit_nlopt->nlopt_algo);
      break;
    case PROP_LOCAL_ALGO:
      g_value_set_enum (value, fit_nlopt->local_nlopt_algo);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
ncm_fit_nlopt_dispose (GObject *object)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (object);

  ncm_vector_clear (&fit_nlopt->lb);
  ncm_vector_clear (&fit_nlopt->ub);
  ncm_vector_clear (&fit_nlopt->pabs);
  ncm_vector_clear (&fit_nlopt->pscale);

  /* Chain up : end */
  G_OBJECT_CLASS (ncm_fit_nlopt_parent_class)->dispose (object);
}

static void
ncm_fit_nlopt_finalize (GObject *object)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (object);
#ifdef HAVE_NLOPT_2_2
  if (fit_nlopt->nlopt != NULL)
    nlopt_destroy (fit_nlopt->nlopt);
  if (fit_nlopt->local_nlopt != NULL)
    nlopt_destroy (fit_nlopt->local_nlopt);
#endif /* HAVE_NLOPT_2_2 */

  if (fit_nlopt->desc != NULL)
    g_free (fit_nlopt->desc);

  /* Chain up : end */
  G_OBJECT_CLASS (ncm_fit_nlopt_parent_class)->finalize (object);
}

static NcmFit *_ncm_fit_nlopt_copy_new (NcmFit *fit, NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype);
static void _ncm_fit_nlopt_reset (NcmFit *fit);
static gboolean _ncm_fit_nlopt_run (NcmFit *fit, NcmFitRunMsgs mtype);
static const gchar *_ncm_fit_nlopt_get_desc (NcmFit *fit);

static void
ncm_fit_nlopt_class_init (NcmFitNLOptClass *klass)
{
  GObjectClass* object_class = G_OBJECT_CLASS (klass);
  NcmFitClass* fit_class     = NCM_FIT_CLASS (klass);

  object_class->constructed  = &_ncm_fit_nlopt_constructed;
  object_class->set_property = &_ncm_fit_nlopt_set_property;
  object_class->get_property = &_ncm_fit_nlopt_get_property;
  object_class->dispose      = &ncm_fit_nlopt_dispose;
  object_class->finalize     = &ncm_fit_nlopt_finalize;

  g_object_class_install_property (object_class,
                                   PROP_ALGO,
                                   g_param_spec_enum ("algorithm",
                                                      NULL,
                                                      "NLOpt algorithm",
                                                      NCM_TYPE_FIT_NLOPT_ALGORITHM, NLOPT_LN_NELDERMEAD,
                                                      G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));
  
  g_object_class_install_property (object_class,
                                   PROP_LOCAL_ALGO,
                                   g_param_spec_enum ("local-algorithm",
                                                      NULL,
                                                      "NLOpt local algorithm",
                                                      NCM_TYPE_FIT_NLOPT_ALGORITHM, NLOPT_LN_NELDERMEAD,
                                                      G_PARAM_READWRITE | G_PARAM_STATIC_NAME | G_PARAM_STATIC_BLURB));


  fit_class->copy_new = &_ncm_fit_nlopt_copy_new;
  fit_class->reset    = &_ncm_fit_nlopt_reset;
  fit_class->run      = &_ncm_fit_nlopt_run;
  fit_class->get_desc = &_ncm_fit_nlopt_get_desc;
}

static NcmFit *
_ncm_fit_nlopt_copy_new (NcmFit *fit, NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (fit);
  if (fit_nlopt->local_nlopt_algo == 0)
    return ncm_fit_nlopt_new (lh, mset, gtype, fit_nlopt->nlopt_algo);
  else
    return ncm_fit_nlopt_local_new (lh, mset, gtype, fit_nlopt->nlopt_algo, fit_nlopt->local_nlopt_algo);
}

static void 
_ncm_fit_nlopt_reset (NcmFit *fit)
{
  /* Chain up : start */
  NCM_FIT_CLASS (ncm_fit_nlopt_parent_class)->reset (fit);
  {
    NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (fit);
    
    if (fit_nlopt->fparam_len != fit->fstate->fparam_len)
    {
      fit_nlopt->fparam_len = fit->fstate->fparam_len;

      ncm_vector_clear (&fit_nlopt->lb);
      ncm_vector_clear (&fit_nlopt->ub);
      ncm_vector_clear (&fit_nlopt->pabs);
      ncm_vector_clear (&fit_nlopt->pscale);

      fit_nlopt->lb = ncm_vector_new (fit_nlopt->fparam_len);
      fit_nlopt->ub = ncm_vector_new (fit_nlopt->fparam_len);
      fit_nlopt->pabs = ncm_vector_new (fit_nlopt->fparam_len);
      fit_nlopt->pscale = ncm_vector_new (fit_nlopt->fparam_len);

#ifdef HAVE_NLOPT_2_2
      if (fit_nlopt->nlopt != NULL)
      {
        nlopt_destroy (fit_nlopt->nlopt);
        fit_nlopt->nlopt = NULL;
      }
#endif /* HAVE_NLOPT_2_2 */
      
      ncm_fit_nlopt_set_algo (fit_nlopt, fit_nlopt->nlopt_algo);
      if (fit_nlopt->local_nlopt_algo != 0)
        ncm_fit_nlopt_set_algo (fit_nlopt, fit_nlopt->local_nlopt_algo);
    }
  }
}

typedef gdouble (*_NcmFitNLOptOldFunc) (gint n, const gdouble *x, gdouble *grad, gpointer userdata);
static gdouble _ncm_fit_nlopt_func (guint n, const gdouble *x, gdouble *grad, gpointer userdata);
static gdouble _ncm_fit_nlopt_func_constraint (guint n, const gdouble *x, gdouble *grad, gpointer userdata);

static gboolean
_ncm_fit_nlopt_run (NcmFit *fit, NcmFitRunMsgs mtype)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (fit);
  gdouble minf = 0.0;

  g_assert (fit->fstate->fparam_len != 0);

  ncm_mset_fparams_get_vector (fit->mset, fit->fstate->fparams);

#ifdef HAVE_NLOPT_2_2
  {
    nlopt_result ret;
    gint i;

    for (i = 0; i < fit_nlopt->fparam_len; i++)
    {
      ncm_vector_set (fit_nlopt->lb,     i, ncm_mset_fparam_get_lower_bound (fit->mset, i));
      ncm_vector_set (fit_nlopt->ub,     i, ncm_mset_fparam_get_upper_bound (fit->mset, i));
      ncm_vector_set (fit_nlopt->pabs,   i, ncm_mset_fparam_get_abstol (fit->mset, i));
      ncm_vector_set (fit_nlopt->pscale, i, ncm_mset_fparam_get_scale (fit->mset, i));
    }

    nlopt_remove_inequality_constraints (fit_nlopt->nlopt);
    nlopt_remove_equality_constraints (fit_nlopt->nlopt);

    for (i = 0; i < ncm_fit_has_inequality_constraints (fit); i++)
    {
      NcmFitConstraint *fitc = g_ptr_array_index (fit->inequality_constraints, i);
      ret = nlopt_add_inequality_constraint (fit_nlopt->nlopt, &_ncm_fit_nlopt_func_constraint, fitc, fitc->tot);
      if (ret < 0)
        g_error ("_ncm_fit_nlopt_run: cannot add inequality constrain: (%d)", ret);
    }

    for (i = 0; i < ncm_fit_has_equality_constraints (fit); i++)
    {
      NcmFitConstraint *fitc = g_ptr_array_index (fit->equality_constraints, i);
      ret = nlopt_add_equality_constraint (fit_nlopt->nlopt, &_ncm_fit_nlopt_func_constraint, fitc, fitc->tot);
      if (ret < 0)
        g_error ("_ncm_fit_nlopt_run: cannot add equality constrain: (%d)", ret);
    }
    
    nlopt_set_min_objective (fit_nlopt->nlopt, &_ncm_fit_nlopt_func, fit);
    nlopt_set_lower_bounds (fit_nlopt->nlopt, ncm_vector_gsl (fit_nlopt->lb)->data);
    nlopt_set_upper_bounds (fit_nlopt->nlopt, ncm_vector_gsl (fit_nlopt->ub)->data);
    
    nlopt_set_ftol_rel (fit_nlopt->nlopt, fit->m2lnL_reltol);
    if (NCM_FIT_DEFAULT_M2LNL_ABSTOL != 0)
      nlopt_set_ftol_abs (fit_nlopt->nlopt, NCM_FIT_DEFAULT_M2LNL_ABSTOL);
    nlopt_set_xtol_rel (fit_nlopt->nlopt, fit->params_reltol);
    nlopt_set_xtol_abs (fit_nlopt->nlopt, ncm_vector_gsl (fit_nlopt->pabs)->data);
    nlopt_set_maxeval (fit_nlopt->nlopt, fit->maxiter);
    nlopt_set_initial_step (fit_nlopt->nlopt, ncm_vector_gsl (fit_nlopt->pscale)->data);
    if (fit_nlopt->local_nlopt != NULL)
    {
      nlopt_set_ftol_rel (fit_nlopt->local_nlopt, fit->m2lnL_reltol);
      if (NCM_FIT_DEFAULT_M2LNL_ABSTOL != 0)
        nlopt_set_ftol_abs (fit_nlopt->local_nlopt, NCM_FIT_DEFAULT_M2LNL_ABSTOL);
      nlopt_set_xtol_rel (fit_nlopt->local_nlopt, fit->params_reltol);
      nlopt_set_xtol_abs (fit_nlopt->local_nlopt, ncm_vector_gsl (fit_nlopt->pabs)->data);
      nlopt_set_maxeval (fit_nlopt->local_nlopt, fit->maxiter);
      ret = nlopt_set_local_optimizer (fit_nlopt->nlopt, fit_nlopt->local_nlopt);
      if (ret < 0)
        g_error ("_ncm_fit_nlopt_run: (%d)", ret);
    }

    ret = nlopt_optimize (fit_nlopt->nlopt, ncm_vector_gsl (fit->fstate->fparams)->data, &minf);

    fit->fstate->m2lnL_prec = nlopt_get_ftol_rel (fit_nlopt->nlopt);
    fit->fstate->params_prec = nlopt_get_xtol_rel (fit_nlopt->nlopt);

    if (ret < 0)
    {
      switch (ret)
      {
        case -2:
          ncm_fit_log_step_error (fit, "Algorithm not supported or inconsistent parameters", ret);
          break;
        default:
          ncm_fit_log_step_error (fit, "(%d)", ret);
          break;
      }
      
    }
  }
#else
  {
    gint ret;
    if (ncm_fit_has_equality_constraints (fit) || ncm_fit_has_inequality_constraints (fit))
      g_error ("_ncm_fit_nlopt_run: Older version of nlopt does not support constraints, update to version >= 2.2");

    ret = nlopt_minimize (algo,
                          fit->fstate->fparam_len,
                          (_NcmFitNLOptOldFunc) &_ncm_fit_nlopt_func, fit,
                          lb, ub,
                          fit->fstate->fparam->data,
                          &minf,
                          -HUGE_VAL,
                          fit->m2lnL_prec, NCM_FIT_DEFAULT_M2LNL_ABSTOL,
                          fit->param_prec, pabs,
                          niters, 0);

    if (ret < 0)
      ncm_fit_log_step_error (fit, "(%d)", ret);
  }
#endif

  fit->fstate->m2lnL = minf;
  ncm_mset_fparams_set_vector (fit->mset, fit->fstate->fparams);

  return TRUE;
}

static gdouble
_ncm_fit_nlopt_func (guint n, const gdouble *x, gdouble *grad, gpointer userdata)
{
  NcmFit *fit = NCM_FIT (userdata);
  gdouble m2lnL;

  fit->fstate->niter++;
  ncm_mset_fparams_set_array (fit->mset, x);

  //ncm_mset_params_log_vals (fit->mset);
  
  if (!ncm_mset_params_valid (fit->mset))
    return GSL_NAN;
  
  if (grad != NULL)
  {
    NcmVector *gradv = ncm_vector_new_data_static (grad, n, 1);
    ncm_fit_m2lnL_val_grad (fit, &m2lnL, gradv);
    ncm_vector_free (gradv);
  }
  else
    ncm_fit_m2lnL_val (fit, &m2lnL);
  
  ncm_fit_log_step (fit, m2lnL);

  return m2lnL;
}

static gdouble
_ncm_fit_nlopt_func_constraint (guint n, const gdouble *x, gdouble *grad, gpointer userdata)
{
  NcmFitConstraint *fitc = NCM_FIT_CONSTRAINT (userdata);
  NcmFit *fit = fitc->fit;
  gdouble constraint;

  fit->fstate->func_eval++;
  ncm_mset_fparams_set_array (fit->mset, x);
  
  if (!ncm_mset_params_valid (fit->mset))
    return GSL_NAN;

  constraint = ncm_mset_func_eval0 (fitc->func, fit->mset);
  
  if (grad != NULL)
  {
    NcmVector *gradv = ncm_vector_new_data_static (grad, n, 1);
    ncm_mset_func_numdiff_fparams (fitc->func, fit->mset, NULL, gradv);
    ncm_vector_scale (gradv, 2.0 * constraint);
    ncm_vector_free (gradv);
  }

  return constraint * constraint;
}

static const gchar *
_ncm_fit_nlopt_get_desc (NcmFit *fit)
{
  NcmFitNLOpt *fit_nlopt = NCM_FIT_NLOPT (fit);
  if (fit_nlopt->desc == NULL)
  {
    GEnumClass *enum_class = g_type_class_ref (NCM_TYPE_FIT_NLOPT_ALGORITHM);
    GEnumValue *res = g_enum_get_value (enum_class, fit_nlopt->nlopt_algo);
    if (fit_nlopt->local_nlopt_algo != 0)
    {
      GEnumValue *local_res = g_enum_get_value (enum_class, fit_nlopt->local_nlopt_algo);
      fit_nlopt->desc = g_strdup_printf ("NLOpt:%s:%s", res->value_nick, local_res->value_nick);
    }
    else
      fit_nlopt->desc = g_strdup_printf ("NLOpt:%s", res->value_nick);
    g_type_class_unref (enum_class);
  }
  return fit_nlopt->desc;
}

/**
 * ncm_fit_nlopt_new:
 * @lh: FIXME
 * @mset: FIXME
 * @gtype: FIXME
 * @algo: FIXME
 *
 * FIXME
 * 
 * Returns: FIXME
 */
NcmFit *
ncm_fit_nlopt_new (NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype, nlopt_algorithm algo)
{
  return g_object_new (NCM_TYPE_FIT_NLOPT, 
                       "likelihood", lh,
                       "mset", mset,
                       "grad-type", gtype,
                       "algorithm", algo,
                       NULL
                       );
}

/**
 * ncm_fit_nlopt_local_new:
 * @lh: FIXME
 * @mset: FIXME
 * @gtype: FIXME
 * @algo: FIXME
 * @local_algo: FIXME
 *
 * FIXME
 * 
 * Returns: FIXME
 */
NcmFit *
ncm_fit_nlopt_local_new (NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype, nlopt_algorithm algo, nlopt_algorithm local_algo)
{
  return g_object_new (NCM_TYPE_FIT_NLOPT, 
                       "likelihood", lh,
                       "mset", mset,
                       "grad-type", gtype,
                       "algorithm", algo,
                       "local-algorithm", local_algo,
                       NULL
                       );
}

/**
 * ncm_fit_nlopt_new_default:
 * @lh: FIXME
 * @mset: FIXME
 * @gtype: FIXME
 *
 * FIXME
 * 
 * Returns: FIXME
 */
NcmFit *
ncm_fit_nlopt_new_default (NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype)
{
  return g_object_new (NCM_TYPE_FIT_NLOPT, 
                       "likelihood", lh,
                       "mset", mset,
                       "grad-type", gtype,
                       NULL
                       );
}

/**
 * ncm_fit_nlopt_new_by_name:
 * @lh: FIXME
 * @mset: FIXME
 * @gtype: FIXME
 * @algo_name: FIXME
 *
 * FIXME
 * 
 * Returns: FIXME
 */
NcmFit *
ncm_fit_nlopt_new_by_name (NcmLikelihood *lh, NcmMSet *mset, NcmFitGradType gtype, gchar *algo_name)
{
  if (algo_name != NULL)
  {
    gchar **algo_names = g_strsplit (algo_name, ":", 2);
    guint nnames = g_strv_length (algo_names); 
    if (nnames == 1)
    {
      const GEnumValue *algo = ncm_cfg_get_enum_by_id_name_nick (NCM_TYPE_FIT_NLOPT_ALGORITHM,
                                                                 algo_name);
      if (algo == NULL)
        g_error ("ncm_fit_nlopt_new_by_name: algorithm %s not found.", algo_name);

      g_strfreev (algo_names);
      return ncm_fit_nlopt_new (lh, mset, gtype, algo->value);
    }
    else if (nnames == 2)
    {
      const GEnumValue *algo = ncm_cfg_get_enum_by_id_name_nick (NCM_TYPE_FIT_NLOPT_ALGORITHM,
                                                                 algo_names[0]);
      const GEnumValue *local_algo = ncm_cfg_get_enum_by_id_name_nick (NCM_TYPE_FIT_NLOPT_ALGORITHM,
                                                                       algo_names[1]);
      if (algo == NULL)
        g_error ("ncm_fit_nlopt_new_by_name: algorithm %s not found.", algo_names[0]);
      if (local_algo == NULL)
        g_error ("ncm_fit_nlopt_new_by_name: algorithm %s not found.", algo_names[1]);

      g_strfreev (algo_names);
      return ncm_fit_nlopt_local_new (lh, mset, gtype, algo->value, local_algo->value);
    }
    else
      g_error ("ncm_fit_nlopt_new_by_name: cannot parse algorithm name ``%s''\n", algo_name);
  }
  else
    return ncm_fit_nlopt_new_default (lh, mset, gtype);
}

/**
 * ncm_fit_nlopt_set_algo: (skip)
 * @fit_nlopt: a #NcmFitNLOpt.
 * @algo: a #nlopt_algorithm.
 *
 * FIXME
 *
 */
void
ncm_fit_nlopt_set_algo (NcmFitNLOpt *fit_nlopt, nlopt_algorithm algo)
{
#ifdef HAVE_NLOPT_2_2
  gboolean nlopt_alloc = FALSE;
  NcmFit *fit = NCM_FIT (fit_nlopt);

  if (fit_nlopt->nlopt == NULL)
    nlopt_alloc = TRUE;

  else if (fit_nlopt->nlopt_algo != algo)
  {
    nlopt_destroy (fit_nlopt->nlopt);
    nlopt_alloc = TRUE;
  }

  if (nlopt_alloc)
  {
    fit_nlopt->nlopt = nlopt_create (algo, fit->fstate->fparam_len);
    fit_nlopt->nlopt_algo = algo;
  }
#endif /* HAVE_NLOPT_2_2 */
  if (fit_nlopt->nlopt_algo != algo)
  {
    fit_nlopt->nlopt_algo = algo;
    if (fit_nlopt->desc != NULL)
      g_free (fit_nlopt->desc);
  }
}

/**
 * ncm_fit_nlopt_set_local_algo: (skip)
 * @fit_nlopt: a #NcmFitNLOpt.
 * @algo: a #nlopt_algorithm.
 *
 * FIXME
 *
 */
void
ncm_fit_nlopt_set_local_algo (NcmFitNLOpt *fit_nlopt, nlopt_algorithm algo)
{
#ifdef HAVE_NLOPT_2_2
  gboolean nlopt_alloc = FALSE;
  NcmFit *fit = NCM_FIT (fit_nlopt);

  if (fit_nlopt->local_nlopt == NULL)
    nlopt_alloc = TRUE;

  else if (fit_nlopt->local_nlopt_algo != algo)
  {
    nlopt_destroy (fit_nlopt->local_nlopt);
    nlopt_alloc = TRUE;
  }

  if (nlopt_alloc)
  {
    fit_nlopt->local_nlopt = nlopt_create (algo, fit->fstate->fparam_len);
    fit_nlopt->local_nlopt_algo = algo;
  }
#endif /* HAVE_NLOPT_2_2 */
  if (fit_nlopt->local_nlopt_algo != algo)
  {
    fit_nlopt->local_nlopt_algo = algo;
    if (fit_nlopt->desc != NULL)
      g_free (fit_nlopt->desc);
  }
}

#endif /* NUMCOSMO_HAVE_NLOPT */
