// nx_curve_sampling.cpp : Defines the exported functions for the DLL.
// This is an example of an exported function.
/*HEAD create_points_along_a_curve CCC UFUN */
/*
This program creates points along a curve either
by distance or chordal tolerance and prints the
point coordinate WCS dependent to info window
*/
#include "nx_curve_sampling.h"
#include <stdio.h>
#include <string.h>
#include <uf.h>
#include <uf_ui.h>
#include <uf_assem.h>
#include <uf_curve.h>
#include <uf_modl.h>
#include <uf_csys.h>


#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))

static int report_error(const char* file, int line, const char* call, int irc)
{
    if (irc)
    {
        char err[133];

        UF_get_fail_message(irc, err);
        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            sprintf(err, "error %d at line %d in %s\n", irc, line, file);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n\n");
        }
        else
        {
            fprintf(stderr, "%s\n", err);
            fprintf(stderr, "error %d at line %d in %s\n", irc, line, file);
            fprintf(stderr, "%s;\n\n", call);
#ifdef _USRDLL
            uc1601(err, TRUE);
            sprintf(err, "error %d at line %d in %s\n", irc, line, file);
            uc1601(err, TRUE);
#endif
        }
    }
    return(irc);
}

static void print_message(const char* text)
{
    if (!UF_UI_open_listing_window())
        UF_UI_write_listing_window(text);
}

/*ARGSUSED*/
static int mask_for_curves(UF_UI_selection_p_t select, void* type)
{
    UF_UI_mask_t
        mask[4] = { { UF_line_type, 0, 0 },
                    { UF_circle_type, 0, 0 },
                    { UF_conic_type, 0, 0 },
                    { UF_spline_type, 0, 0 } };

    if (!UF_CALL(UF_UI_set_sel_mask(select,
        UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))
        return (UF_UI_SEL_SUCCESS);
    else
        return (UF_UI_SEL_FAILURE);
}

extern tag_t select_a_curve(const char* prompt)
{
    int
        resp;
    double
        cp[3];
    tag_t
        object,
        view;

    UF_CALL(UF_UI_select_with_single_dialog((char*)"Select a curve", (char*)prompt,
        UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,
        &object, cp, &view));

    if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)
    {
        UF_CALL(UF_DISP_set_highlight(object, 0));
        return object;
    }
    else return NULL_TAG;

}

static void do_it(void)
{
    tag_t point, curve, part;
    char text[133];
    char prompt[][16] = { "Chord", "Angle", "Point distance" };
    int junk, resp, np;
    int j, k;
    int input_csys = UF_CSYS_ROOT_COORDS;
    int output_csys = UF_CSYS_ROOT_WCS_COORDS;
    double tols[] = { 0.1, 0.5, 1.0 };
    double* pts;
    double input_point[3], output_point[3];


    if ((part = UF_ASSEM_ask_work_part()) == NULL_TAG)
    {
        print_message("No work part. Exiting...\n");
        return;
    }

    // Ask for parameters
    resp = uc1609("Parameter for point creation", prompt, 3, tols, &junk);
    if (resp != 3 && resp != 4) return;

    while (curve = select_a_curve("Select a curve"))
    {
        UF_CALL(UF_MODL_ask_curve_points(curve, tols[0], tols[1], tols[2], &np, &pts));
        sprintf(text, "\nCount of Points:%d\n", np);
        print_message(text);
        print_message("\nWCS coordinates of Points:\n");

        for (j = 0; j < np; j++)
        {
            k = j * 3; /* 3 Koordinaten */
            UF_CALL(UF_CURVE_create_point(&pts[k], &point));
            input_point[0] = pts[k];
            input_point[1] = pts[k + 1];
            input_point[2] = pts[k + 2];
            UF_CSYS_map_point(input_csys, input_point, output_csys, output_point);
            sprintf(text, "  %9.3f %9.3f %9.3f\n", output_point[0], output_point[1], output_point[2]);
            print_message(text);
        } // for

        UF_free(pts);

    } // while

} // do_it

/*ARGSUSED*/
NXCURVESAMPLING_API void ufusr(char* param, int* retcode, int paramLen)
{
    if (UF_CALL(UF_initialize())) return;
    do_it();
    UF_terminate();
}

NXCURVESAMPLING_API int ufusr_ask_unload(void)
{
    return (UF_UNLOAD_IMMEDIATELY);
}