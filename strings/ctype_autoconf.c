/* This file is generated automatically by configure. */

/* declarations for the big5 character set, filled in by configure */
extern uchar ctype_big5[], to_lower_big5[], to_upper_big5[], sort_order_big5[];
extern int     my_strcoll_big5(const uchar *, const uchar *);
extern int     my_strxfrm_big5(uchar *, const uchar *, int);
extern int     my_strnncoll_big5(const uchar *, int, const uchar *, int);
extern int     my_strnxfrm_big5(uchar *, const uchar *, int, int);
extern my_bool my_like_range_big5(const char *, uint, pchar, uint,
                          char *, char *, uint *, uint *);
extern int     ismbchar_big5(const char *, const char *);
extern my_bool ismbhead_big5(uint);
extern int     mbcharlen_big5(uint);

/* declarations for the czech character set, filled in by configure */
extern uchar ctype_czech[], to_lower_czech[], to_upper_czech[], sort_order_czech[];
extern int     my_strcoll_czech(const uchar *, const uchar *);
extern int     my_strxfrm_czech(uchar *, const uchar *, int);
extern int     my_strnncoll_czech(const uchar *, int, const uchar *, int);
extern int     my_strnxfrm_czech(uchar *, const uchar *, int, int);
extern my_bool my_like_range_czech(const char *, uint, pchar, uint,
                          char *, char *, uint *, uint *);

/* declarations for the euc_kr character set, filled in by configure */
extern uchar ctype_euc_kr[], to_lower_euc_kr[], to_upper_euc_kr[], sort_order_euc_kr[];
extern int     ismbchar_euc_kr(const char *, const char *);
extern my_bool ismbhead_euc_kr(uint);
extern int     mbcharlen_euc_kr(uint);

/* declarations for the gb2312 character set, filled in by configure */
extern uchar ctype_gb2312[], to_lower_gb2312[], to_upper_gb2312[], sort_order_gb2312[];
extern int     ismbchar_gb2312(const char *, const char *);
extern my_bool ismbhead_gb2312(uint);
extern int     mbcharlen_gb2312(uint);

/* declarations for the gbk character set, filled in by configure */
extern uchar ctype_gbk[], to_lower_gbk[], to_upper_gbk[], sort_order_gbk[];
extern int     my_strcoll_gbk(const uchar *, const uchar *);
extern int     my_strxfrm_gbk(uchar *, const uchar *, int);
extern int     my_strnncoll_gbk(const uchar *, int, const uchar *, int);
extern int     my_strnxfrm_gbk(uchar *, const uchar *, int, int);
extern my_bool my_like_range_gbk(const char *, uint, pchar, uint,
                          char *, char *, uint *, uint *);
extern int     ismbchar_gbk(const char *, const char *);
extern my_bool ismbhead_gbk(uint);
extern int     mbcharlen_gbk(uint);

/* declarations for the sjis character set, filled in by configure */
extern uchar ctype_sjis[], to_lower_sjis[], to_upper_sjis[], sort_order_sjis[];
extern int     my_strcoll_sjis(const uchar *, const uchar *);
extern int     my_strxfrm_sjis(uchar *, const uchar *, int);
extern int     my_strnncoll_sjis(const uchar *, int, const uchar *, int);
extern int     my_strnxfrm_sjis(uchar *, const uchar *, int, int);
extern my_bool my_like_range_sjis(const char *, uint, pchar, uint,
                          char *, char *, uint *, uint *);
extern int     ismbchar_sjis(const char *, const char *);
extern my_bool ismbhead_sjis(uint);
extern int     mbcharlen_sjis(uint);

/* declarations for the tis620 character set, filled in by configure */
extern uchar ctype_tis620[], to_lower_tis620[], to_upper_tis620[], sort_order_tis620[];
extern int     my_strcoll_tis620(const uchar *, const uchar *);
extern int     my_strxfrm_tis620(uchar *, const uchar *, int);
extern int     my_strnncoll_tis620(const uchar *, int, const uchar *, int);
extern int     my_strnxfrm_tis620(uchar *, const uchar *, int, int);
extern my_bool my_like_range_tis620(const char *, uint, pchar, uint,
                          char *, char *, uint *, uint *);

/* declarations for the ujis character set, filled in by configure */
extern uchar ctype_ujis[], to_lower_ujis[], to_upper_ujis[], sort_order_ujis[];
extern int     ismbchar_ujis(const char *, const char *);
extern my_bool ismbhead_ujis(uint);
extern int     mbcharlen_ujis(uint);

CHARSET_INFO compiled_charsets[] = {

  /* this information is filled in by configure */
  {
    8,    /* number */
    "latin1",     /* name */
    ctype_latin1,
    to_lower_latin1,
    to_upper_latin1,
    sort_order_latin1,
    0,          /* strxfrm_multiply */
    NULL,       /* strcoll    */
    NULL,       /* strxfrm    */
    NULL,       /* strnncoll  */
    NULL,       /* strnxfrm   */
    NULL,       /* like_range */
    0,          /* mbmaxlen  */
    NULL,       /* ismbchar  */
    NULL,       /* ismbhead  */
    NULL        /* mbcharlen */
  },

  /* this information is filled in by configure */
  {
    1,    /* number */
    "big5",     /* name */
    ctype_big5,
    to_lower_big5,
    to_upper_big5,
    sort_order_big5,
    1,      /* strxfrm_multiply */
    my_strcoll_big5,
    my_strxfrm_big5,
    my_strnncoll_big5,
    my_strnxfrm_big5,
    my_like_range_big5,
    2,      /* mbmaxlen */
    ismbchar_big5,
    ismbhead_big5,
    mbcharlen_big5
  },

  /* this information is filled in by configure */
  {
    2,    /* number */
    "czech",     /* name */
    ctype_czech,
    to_lower_czech,
    to_upper_czech,
    sort_order_czech,
    4,      /* strxfrm_multiply */
    my_strcoll_czech,
    my_strxfrm_czech,
    my_strnncoll_czech,
    my_strnxfrm_czech,
    my_like_range_czech,
    0,          /* mbmaxlen  */
    NULL,       /* ismbchar  */
    NULL,       /* ismbhead  */
    NULL        /* mbcharlen */
  },

  /* this information is filled in by configure */
  {
    19,    /* number */
    "euc_kr",     /* name */
    ctype_euc_kr,
    to_lower_euc_kr,
    to_upper_euc_kr,
    sort_order_euc_kr,
    0,          /* strxfrm_multiply */
    NULL,       /* strcoll    */
    NULL,       /* strxfrm    */
    NULL,       /* strnncoll  */
    NULL,       /* strnxfrm   */
    NULL,       /* like_range */
    2,      /* mbmaxlen */
    ismbchar_euc_kr,
    ismbhead_euc_kr,
    mbcharlen_euc_kr
  },

  /* this information is filled in by configure */
  {
    24,    /* number */
    "gb2312",     /* name */
    ctype_gb2312,
    to_lower_gb2312,
    to_upper_gb2312,
    sort_order_gb2312,
    0,          /* strxfrm_multiply */
    NULL,       /* strcoll    */
    NULL,       /* strxfrm    */
    NULL,       /* strnncoll  */
    NULL,       /* strnxfrm   */
    NULL,       /* like_range */
    2,      /* mbmaxlen */
    ismbchar_gb2312,
    ismbhead_gb2312,
    mbcharlen_gb2312
  },

  /* this information is filled in by configure */
  {
    28,    /* number */
    "gbk",     /* name */
    ctype_gbk,
    to_lower_gbk,
    to_upper_gbk,
    sort_order_gbk,
    1,      /* strxfrm_multiply */
    my_strcoll_gbk,
    my_strxfrm_gbk,
    my_strnncoll_gbk,
    my_strnxfrm_gbk,
    my_like_range_gbk,
    2,      /* mbmaxlen */
    ismbchar_gbk,
    ismbhead_gbk,
    mbcharlen_gbk
  },

  /* this information is filled in by configure */
  {
    13,    /* number */
    "sjis",     /* name */
    ctype_sjis,
    to_lower_sjis,
    to_upper_sjis,
    sort_order_sjis,
    1,      /* strxfrm_multiply */
    my_strcoll_sjis,
    my_strxfrm_sjis,
    my_strnncoll_sjis,
    my_strnxfrm_sjis,
    my_like_range_sjis,
    2,      /* mbmaxlen */
    ismbchar_sjis,
    ismbhead_sjis,
    mbcharlen_sjis
  },

  /* this information is filled in by configure */
  {
    18,    /* number */
    "tis620",     /* name */
    ctype_tis620,
    to_lower_tis620,
    to_upper_tis620,
    sort_order_tis620,
    4,      /* strxfrm_multiply */
    my_strcoll_tis620,
    my_strxfrm_tis620,
    my_strnncoll_tis620,
    my_strnxfrm_tis620,
    my_like_range_tis620,
    0,          /* mbmaxlen  */
    NULL,       /* ismbchar  */
    NULL,       /* ismbhead  */
    NULL        /* mbcharlen */
  },

  /* this information is filled in by configure */
  {
    12,    /* number */
    "ujis",     /* name */
    ctype_ujis,
    to_lower_ujis,
    to_upper_ujis,
    sort_order_ujis,
    0,          /* strxfrm_multiply */
    NULL,       /* strcoll    */
    NULL,       /* strxfrm    */
    NULL,       /* strnncoll  */
    NULL,       /* strnxfrm   */
    NULL,       /* like_range */
    3,      /* mbmaxlen */
    ismbchar_ujis,
    ismbhead_ujis,
    mbcharlen_ujis
  },

  /* this information is filled in by configure */
  {
    0,          /* end-of-list marker */
    NullS,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    NULL,
    NULL,
    NULL
  }
};
