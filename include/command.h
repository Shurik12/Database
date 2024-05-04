 #include "list.h"


enum cmd_type
{
    CMD_NONE, QUIT, INSERT, SELECT, DELETE
};

enum op_type
{
    OP_NONE, AND, OR
};

enum cond_type
{
    COND_NONE, EQ, NE, LT, GT, LE, GE, LIKE
};

class command : public record
{
    private :
        cmd_type type;
        cond_type c_name;
        cond_type c_phone;
        cond_type c_group;
        op_type op;

    public :
        command() : record()
        {
            type = CMD_NONE;
            c_name = COND_NONE;
            c_phone = COND_NONE;
            c_group = COND_NONE;
            op = OP_NONE;
        }
        ~command();
        int parse(char * , char **);
        int check_cond(cond_type ,const char * , const char *);
        int check_cond(cond_type ,const int , const int);
        int check_like(const char *);
        int condition(const record * date);
        int condition_phone(const record * date);
        int condition_group(const record * date);
        int condition_name(const record * date);

        cond_type init_c(char *);
        void init_insert(char **);
        void parse_where(char ** , int);
        int init(char * );

        void apply_insert(list *);
        void apply_select(list *);
        void apply_delete(list *);
        int apply(list *);

        void check();

		int search_equal(list * , list_node *);

        list_node * direction_phone(list * , list_node * );
        void search_select_phone(list *,list_node *);
        void search_delete_phone(list *);
		int get_hash_key(int);

        void search_select_group(list *,list_node *);
        list_node * direction_group(list * ,list_node * );
        void search_delete_group(list * , list_node *);

        void search_delete_name(list *, list_node *);
        void search_select_name(list *, list_node *);

        list_node * direction_name(list_node * );
};
