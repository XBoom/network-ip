#ifndef __RB_TREE__
#define __RB_TREE__

#define RB_ENTRY(type)                                \
    struct                                            \
    {                                                 \
        struct type *rbe_left;   /* left element */   \
        struct type *rbe_right;  /* right element */  \
        struct type *rbe_parent; /* parent element */ \
        int rbe_color;           /* node color */     \
    }

#endif