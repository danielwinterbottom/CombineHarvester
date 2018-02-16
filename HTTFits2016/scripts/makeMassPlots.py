import os

CHN_DICT = {
    "et": "e_{}#tau_{h}",
    "mt": "#mu_{}#tau_{h}",
    "em": "e#mu",
    "tt": "#tau_{h}#tau_{h}"
}

CAT_DICT = {
    "1": "",
}

RANGE_DICT = {
    "em" : 1E-5 ,
    "et" : 5E-7 ,
    "mt" : 1E-7 , 
    "tt" : 1E-7 
}

PAD_DICT = {
    "em" : 0.42 ,
    "et" : 0.45 ,
    "mt" : 0.45 , 
    "tt" : 0.5 
}


for MODE in ['prefit', 'postfit']:
    for CHN in ['tt']:
        for CAT in ['1']:
            LABEL = "%s %s" % (CHN_DICT[CHN], CAT_DICT[CAT])
            YMIN = "%s" % RANGE_DICT[CHN]
            PAD = "%s" % PAD_DICT[CHN]
            os.system(('python scripts/postFitPlot.py' \
                  ' --file=shapes.root --ratio --extra_pad="%(PAD)s" --no_signal' \
                  ' --file_dir="htt_%(CHN)s_%(CAT)s" --custom_x_range --x_axis_min=0.1 --x_axis_max 1E4' \
                  ' --ratio_range 0.7,1.3 --empty_bin_error' \
                  ' --outname htt_%(CHN)s_%(CAT)s --mode %(MODE)s --log_y --custom_y_range --y_axis_min "%(YMIN)s" ' \
                  ' --channel_label "%(LABEL)s"' % vars()))

for MODE in ['prefit', 'postfit']:
    for CHN in ['tt']:
        for CAT in ['1']:
            LABEL = "%s %s" % (CHN_DICT[CHN], CAT_DICT[CAT])
            os.system(('python scripts/postFitPlot.py' \
                  ' --file=shapes.root --ratio --extra_pad=0.6 --no_signal' \
                  ' --file_dir="htt_%(CHN)s_%(CAT)s"' \
                  ' --ratio_range 0.7,1.3 ' \
                  ' --outname htt_%(CHN)s_%(CAT)s --mode %(MODE)s' \
                  ' --channel_label "%(LABEL)s"' % vars()))


#python scripts/postFitPlot.py --file=shapes.root --ratio --extra_pad=0.6 --no_signal --file_dir="htt_tt_1" --ratio_range 0.7,1.3  --outname test --mode postfit  --channel_label "#tau_h#tau_h"'

