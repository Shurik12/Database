#include "command.h"
#include <string.h>
#include <stdlib.h>


command :: ~command() {
    //if (name){
    //    delete [] name;
        name = 0;
    //}
}

#define LEN 100

int command :: check_like(const char * date) {
    char  buf[LEN];
    char * word = 0;
    int i = 0;

    for ( i = 0; date[i]; i++)
        buf[i] = date[i];

    buf[i+1] = 0;

    word = strtok(buf , "%_");

    while (word != NULL)
    {
        if (buf[strlen(word)] == '_') check_like(&(buf[strlen(word)+1]));
    }


    return 0;
}

int command :: check_cond(cond_type type, const char * puttern, const char * date) {
    switch(type)
    {
       case COND_NONE :
           return 0;

       case EQ : return strcmp(puttern, date) == 0;
       case NE : return strcmp(puttern, date) != 0;
       case GT : return strcmp(puttern, date) < 0;
       case LT : return strcmp(puttern, date) > 0;
       case GE : return strcmp(puttern, date) <= 0;
       case LE : return strcmp(puttern, date) >= 0;
       case LIKE : return 0;

    }
    return 0;
}


int command :: check_cond(cond_type type, const int pattern, const int  date) {
    switch(type)
    {
       case COND_NONE :
           return 0;

       case EQ : return pattern == date;
       case NE : return pattern != date;
       case GT : return date > pattern;
       case LT : return date < pattern;
       case GE : return date >= pattern;
       case LE : return date <= pattern;
       case LIKE : return 0;

    }
    return 0;
}

int command :: condition_phone(const record * date) {
    return check_cond(c_phone, phone , date -> get_phone());
}

int command :: condition_group(const record * date) {
    return check_cond(c_group, group , date -> get_group());
}

int command :: condition_name(const record * date) {
    return check_cond(c_name, name , date -> get_name());
}

int command :: condition(const record * date) {
    int r_n , r_p, r_g, r = 0;

    if (type != SELECT && type != DELETE)
        return 0;

    if (type == DELETE && c_name == COND_NONE && c_phone == COND_NONE && c_group == COND_NONE)
        return 1;



    r_n = check_cond(c_name, name, date -> get_name());
    r_p = check_cond(c_phone, phone, date -> get_phone());
    r_g = check_cond(c_group, group, date -> get_group());

    r = r_n + r_g + r_p;

    switch(op)
    {
        case OP_NONE : return r;
        case AND : return r == 2;
        case OR : return (r == 2) || (r == 1);
    }
     return 0;
}

#define BUF_LEN 1024

int command :: parse(char * buf, char ** words) {
    char * s = strtok(buf , ", () ; \n \r") ;
    int i = 0;

    if ( s == 0) return -1;

    words[i] = s;
    i++;
    while( s != 0)
    {
        words[i] = (s = strtok(NULL , ", ( ) ; \n \r"));
        i++;
    }

    return i;
}

void command :: init_insert(char ** words) {
    name = words[0];
    phone = atoi(words[1]);
    group = atoi(words[2]);
}

cond_type command :: init_c(char * word) {
    if (strcmp(word, "=") == 0) return EQ;
    if (strcmp(word, "<>") == 0) return NE;
    if (strcmp(word, ">") == 0) return GT;
    if (strcmp(word, "<") == 0) return LT;
    if (strcmp(word, ">=") == 0) return GE;
    if (strcmp(word, "<=") == 0) return LE;
    if (strcmp(word, "like") == 0) return LIKE;

    return COND_NONE;
}


void command :: parse_where(char ** words , int k) {

    if (k <= 0) return ;

    if (strcmp(words[0], "name") == 0) {
        c_name = init_c(words[1]);
        name = words[2];
    } else {
        if (strcmp(words[0], "phone") == 0) {
            c_phone = init_c(words[1]);
            phone = atoi(words[2]);
        } else {
            if (strcmp(words[0], "group") == 0)
            {
                c_group = init_c(words[1]);
                group = atoi(words[2]);
            }
        }
    }

    k-=3;

    if (k > 0) {
         //�������� ������� �������
         if (strcmp(words[3], "and") == 0) op = AND;
         else if (strcmp(words[3], "or") == 0) op = OR;
         parse_where(words+4, k - 1);
    }
}


void command :: check() {
    printf("name : %s\n", name);
    printf("phone : %d\n", phone);
    printf("group : %d\n",  group);
    printf("type : %d\n", type);
    printf("c_name : %d\n", c_name);
    printf("c_phone : %d\n", c_phone);
    printf("c_group : %d\n", c_group);
    printf("op : %d\n", op);
}

int command :: init(char * str) {
    char * words[BUF_LEN];
    int k = 0;
	k = parse(str, words);
    k--;
    if (k < 0) return -1;

    if (strcmp(words[0],"quit") == 0) {
        type = QUIT;
    } else {
        if (strcmp(words[0], "insert") == 0) {
            if (k != 4) return -1;
            type = INSERT;
            init_insert(words+1);
        } else {
            if (strcmp(words[0], "select") == 0) {
                 type = SELECT;
                 if (k > 2) parse_where(words + 3 , k - 3);
            } else {
                 type = DELETE;
                 parse_where(words + 2, k - 2);
            }
        }
    }
    return 0;
}


void command :: apply_insert(list * l) {
    list_node * tmp = new list_node(name, phone, group);
	int k;
	k = search_equal(l, tmp);
	if (k == 0) {
    l -> insert_node_phone(tmp);
    tmp -> set_next(l -> get_head());
    l -> get_head() -> set_prev(tmp);
    tmp -> set_color_name(1);
    l -> set_head(tmp);
    //l -> insert_node_phone(tmp);
    l -> insert_node_group(tmp);
    l -> add_node_name(tmp);
	//tmp -> print();
	}
	tmp = 0;
    delete tmp;
}

/*-------------------------------------Hash phone-----------------------------------------------------------------------*/
int command :: get_hash_key(int phone) {
	int i;
	for (i = 0; phone >= 100; i++)
		phone = phone/10;
	return phone;
}

void command :: search_delete_phone(list * l) {
    //int k;
	list_node *p, *q;
	//k=this->get_phone();
	//k=(k-k%100000)/100000;
	q=l->get_headhashs(get_hash_key(this->get_phone()));
	while (q) {
		p=q->get_next_phone();
		if (this->c_phone==EQ && q->get_phone() == this->get_phone() ) {
			//q->print();
			l->delete_node_name(q);
			l->delete_node_group(q);
			l->delete_node_phone(q);
			delete q;
		}
		q=p;
	}
}

void command :: search_select_phone(list * l, list_node * q) {
	//int k;
	list_node *p;
	//k=this->get_phone();
	//k=(k-k%100000)/100000;
	q=l->get_headhashs(get_hash_key(this->get_phone()));
	while (q) {
		p=q->get_next_phone();
        if (this->c_phone == EQ && q->get_phone() == this->get_phone() ) {
            q->print();
        }
		q=p;
	}
}

int command :: search_equal(list * l, list_node * q)
{
    //int k;
    list_node *p;
    //k=q->get_phone();
    //k=(k-k%100000)/100000;
    p=l->get_headhashs(get_hash_key(this->get_phone()));
    while (p)
    {
        if (q->get_phone() == p->get_phone() && q->get_group() == p->get_group() && strcmp(q->get_name() , p->get_name())==0)
        {return -1;}
        p=p->get_next_phone();
    }
    return 0;
}
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------AVL tree group----------------------------------------------------------------------*/
void command :: search_select_group(list * l, list_node * root)
{
    list_node * tmp = 0;
    if (root != 0)
    switch (c_group)
    {
       case COND_NONE : break;
       case LIKE : break;
       case EQ :
           if (condition_group(root))
           {
               if (op == OP_NONE)
               for (tmp = root; tmp; tmp = tmp -> get_brother_next_group())
                  tmp -> print();

               if (op == AND)
               {
                   for (tmp = root; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();
               }

           }
           else
           {
               if (group < root -> get_group()) search_select_group(l, root -> get_left_group());
               if (group > root -> get_group()) search_select_group(l ,root -> get_right_group());
           }
           break;

       case NE :
           if (condition_group(root))
           {
               if (op == OP_NONE)
                  for (tmp = root; tmp; tmp = tmp -> get_brother_next_group())
                     tmp -> print();

               if (op == AND)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();
               }

               search_select_group(l, root -> get_left_group());
               search_select_group(l, root -> get_right_group());

           }
           else
           {
               if (op == OP_NONE)
               {
                   l -> print_subtree_group(root -> get_left_group());
                   l -> print_subtree_group(root -> get_right_group());
               }
               if (op == AND)
               {
                   search_select_group(l, root -> get_left_group());
                   search_select_group(l, root -> get_right_group());
               }
           }
           break;

       case LT :
           if (condition_group(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_group())
                       tmp -> print();

                   l -> print_subtree_group(root -> get_left_group());

               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();

                   search_select_group(l, root -> get_left_group());
               }
               search_select_group(l, root -> get_right_group());
           }
           else
               search_select_group(l, root -> get_left_group());
           break;

       case GT :
           if (condition_group(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_group())
                      tmp -> print();

                      l -> print_subtree_group(root -> get_right_group());
               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();

                   search_select_group(l, root -> get_right_group());
               }
               search_select_group(l, root -> get_left_group());
           }
           else
               search_select_group(l, root -> get_right_group());
           break;

       case LE :
           if (condition_group(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_group())
                       tmp -> print();

                   l -> print_subtree_group(root -> get_left_group());

               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();

                   search_select_group(l, root -> get_left_group());
               }
               search_select_group(l, root -> get_right_group());
           }
           else
               search_select_group(l, root -> get_left_group());
           break;

       case GE :
           if (condition_group(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_group())
                      tmp -> print();

                      l -> print_subtree_group(root -> get_right_group());
               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_group())
                      if (condition(tmp)) tmp -> print();

                   search_select_group(l, root -> get_right_group());
               }
               search_select_group(l, root -> get_left_group());
           }
           else
               search_select_group(l, root -> get_right_group());
           break;
    }


}


list_node * command :: direction_group(list * l, list_node * root)
{
    switch(c_group)
    {
        case COND_NONE : break;
        case LIKE : break;
        case EQ :
            if (root -> get_group() > group) return root -> get_left_group();
            if (root -> get_group() < group) return root -> get_right_group();
            else
            {
                if (condition(root)) return root;
                else return 0;
            }

        case NE :
            if (type == SELECT)
            {
                search_select_group(l , root -> get_left_group());
                search_select_group(l , root -> get_right_group());
            }
            else
            {
                search_delete_group(l , root -> get_left_group());
                search_delete_group(l , root -> get_right_group());
            }
            break;
        case LT :
            return root -> get_left_group();
            break;
        case GT :
            return root -> get_right_group();
            break;
        case LE :
            return root -> get_left_group();
            break;
        case GE :
            return root -> get_right_group();
            break;
     }
     return root;
}

void command :: search_delete_group(list * l , list_node * root)
{
    list_node * tmp = 0;
    list_node * next = 0;
    //int i;
    while (root != 0)
    {
        if (condition_group(root))
        {
            if (condition(root))
            {

               if (root == l -> get_head()) l -> set_head(root -> get_next());
               if (root -> get_prev()) (root -> get_prev()) -> set_next(root -> get_next());
               if (root -> get_next()) (root -> get_next()) -> set_prev(root -> get_prev());


                tmp = root -> get_brother_next_group();
                if (tmp == 0)
                {
                    tmp = l -> delete_node_group(root);
                    l -> delete_node_name(root);
                    l -> delete_node_phone(root);
                    delete root;


                    root = tmp;
                    tmp = 0;

                }
                else
                {
                    //root -> print();
                    if (root == l -> get_root_group()) l -> set_root_group(tmp);
                    (tmp) -> set_parent_group(root -> get_parent_group());
                    if (root -> get_parent_group())
                    {
                        if (root == (root -> get_parent_group()) -> get_left_group())
                            (root -> get_parent_group()) -> set_left_group(root -> get_brother_next_group());
                        else (root -> get_parent_group()) -> set_right_group(root -> get_brother_next_group());
                    }
                    (tmp) -> set_brother_prev_group(0);
                    (tmp) -> set_left_group(root -> get_left_group());
                    if (root -> get_left_group()) root -> get_left_group() -> set_parent_group(tmp);
                    (tmp) -> set_right_group(root -> get_right_group());
                    if (root -> get_right_group()) root -> get_right_group() -> set_parent_group(tmp);

                    l -> delete_node_name(root);
                    l -> delete_node_phone(root);


                    delete root;

                    root = tmp;
                }
            }
            else
            {
                tmp = root -> get_brother_next_group();
                while (tmp != 0)
                {
                    if (condition(tmp))
                    {
                        //printf("!!\n");
                        (tmp -> get_brother_prev_group()) -> set_brother_next_group(tmp -> get_brother_next_group());

                        if (tmp -> get_brother_next_group())
                            tmp -> get_brother_next_group() -> set_brother_prev_group(tmp -> get_brother_prev_group());


                        if (tmp == l -> get_head()) l -> set_head(tmp -> get_next());
                        if (tmp -> get_next()) tmp -> get_next() -> set_prev(tmp -> get_prev());
                        if (tmp -> get_prev()) tmp -> get_prev() -> set_next(tmp -> get_next());
                        next = tmp -> get_brother_next_group();
                        //if (c_name == COND_NONE && c_group != COND_NONE)
                        {
                            l -> delete_node_name(tmp);
                            l -> delete_node_phone(tmp);
                            delete tmp;
                        }
                        tmp = next;
                    }
                    else tmp = tmp -> get_brother_next_group();
                }
                root = direction_group(l , root);
            }
        }
        else
            root = direction_group(l , root);

    }
}
/*-----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------RB-tree name----------------------------------------------------------------*/
void command :: search_select_name(list * l, list_node * root)
{
    list_node * tmp = 0;
    //if (condition(date)) date -> print() ;
    if (root != 0)
    switch (c_name)
    {
       case COND_NONE : break;
       case LIKE : break;
       case EQ :
           if (condition_name(root))
           {
               if (op == OP_NONE)
               for (tmp = root; tmp; tmp = tmp -> get_brother_next_name())
                  tmp -> print();

               if (op == AND)
               {
                   for (tmp = root; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();
               }

           }
           else
           {
               if (strcmp(name , root -> get_name()) < 0) search_select_name(l, root -> get_left_name());
               if (strcmp(name, root -> get_name()) > 0) search_select_name(l ,root -> get_right_name());
           }
           break;

       case NE :
           if (condition_name(root))
           {
               if (op == OP_NONE)
                  for (tmp = root; tmp; tmp = tmp -> get_brother_next_name())
                     tmp -> print();

               if (op == AND)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();
               }

               search_select_name(l, root -> get_left_name());
               search_select_name(l, root -> get_right_name());

           }
           else
           {
               if (op == OP_NONE)
               {
                   l -> print_subtree_name(root -> get_left_name());
                   l -> print_subtree_name(root -> get_right_name());
               }
               if (op == AND)
               {
                   search_select_name(l, root -> get_left_name());
                   search_select_name(l, root -> get_right_name());
               }
           }
           break;

       case LT :
           if (condition_name(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_name())
                       tmp -> print();

                   l -> print_subtree_name(root -> get_left_name());

               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();

                   search_select_name(l, root -> get_left_name());
               }
               search_select_name(l, root -> get_right_name());
           }
           else
               search_select_name(l, root -> get_left_name());
           break;

       case GT :
           if (condition_name(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_name())
                      tmp -> print();

                      l -> print_subtree_name(root -> get_right_name());
               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();

                   search_select_name(l, root -> get_right_name());
               }
               search_select_name(l, root -> get_left_name());
           }
           else
               search_select_name(l, root -> get_right_name());
           break;

       case LE :
           if (condition_name(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_name())
                       tmp -> print();

                   l -> print_subtree_name(root -> get_left_name());

               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();

                   search_select_name(l, root -> get_left_name());
               }
               search_select_name(l, root -> get_right_name());
           }
           else
               search_select_name(l, root -> get_left_name());
           break;

       case GE :
           if (condition_name(root))
           {
               if (op == OP_NONE)
               {
                   for (tmp = root ; tmp ; tmp = tmp -> get_brother_next_name())
                      tmp -> print();

                      l -> print_subtree_name(root -> get_right_name());
               }
               if (op == AND)
               {
                   for (tmp = root  ; tmp ; tmp = tmp -> get_brother_next_name())
                      if (condition(tmp)) tmp -> print();

                   search_select_name(l, root -> get_right_name());
               }
               search_select_name(l, root -> get_left_name());
           }
           else
               search_select_name(l, root -> get_right_name());
           break;
    }
}


list_node * command :: direction_name(list_node * root)
{
    switch(c_name)
    {
        case COND_NONE : break;
        case LIKE : break;
        case EQ :
            if (strcmp( root -> get_name(), name) > 0) return root -> get_left_name();
            if (strcmp(root -> get_name() , name) < 0) return root -> get_right_name();
            else
            {
                if (condition(root))
                  return root;
                else
                   return 0;
            }
        case NE :
            /*if (type == DELETE)
            {
                search_delete_name(l , root -> get_left_name());
                serch_delete_name(l , root -> get_right_name());
            }
            else
            {
                search_select_name(l , root -> get_left_name());
                serch_select_name(l , root -> get_right_name());
            } */
            break;
        case LT :
            return root -> get_left_name();
            break;
        case GT :
            return root -> get_right_name();
            break;
        case LE :
            return root -> get_left_name();
            break;
        case GE :
            return root -> get_right_name();
            break;
     }
     return root;
}

void command :: search_delete_name(list * l , list_node * root)
{
    //list_node * root = l -> get_root_name();
    list_node * tmp = 0;
    list_node * next = 0;
    list_node * prev = 0;


    while (root != 0)
    {
        /*printf("&\n");
        l -> print_tree_name(l -> get_root_name() , i);
        printf("&\n");*/
        if (condition_name(root))
        {
            if (condition(root))
            {
                next = root -> get_next();
                prev = root -> get_prev();

                if (root == l -> get_head()) l -> set_head(root -> get_next());
                if (next) (next) -> set_prev(prev);
                if (prev) (prev) -> set_next(next);


                tmp = root -> get_brother_next_name();

                if (tmp == 0)
                {
                    tmp = l -> delete_node_name(root);
                    l -> delete_node_group(root);
                    l -> delete_node_phone(root);
                    delete root;

                    root = tmp;
                    tmp = 0;

                }
                else
                {
                    if (root == l -> get_root_name()) l -> set_root_name(tmp);
                    (tmp) -> set_parent_name(root -> get_parent_name());
                    if (root -> get_parent_name())
                    {
                        if (root == (root -> get_parent_name()) -> get_left_name())
                            (root -> get_parent_name()) -> set_left_name(root -> get_brother_next_name());
                        else (root -> get_parent_name()) -> set_right_name(root -> get_brother_next_name());
                    }
                    (tmp) -> set_brother_prev_name(0);
                    (tmp) -> set_left_name(root -> get_left_name());
                    if (root -> get_left_name()) root -> get_left_name() -> set_parent_name(tmp);
                    (tmp) -> set_right_name(root -> get_right_name());
                    if (root -> get_right_name()) root -> get_right_name() -> set_parent_name(tmp);

                    l -> delete_node_group(root);
                    l -> delete_node_phone(root);
                    delete root;

                    root = tmp;
                }
            }
            else
            {
                tmp = root -> get_brother_next_name();
                //root -> print();
                //tmp -> print();
                while (tmp != 0)
                {
                    if (condition(tmp))
                    {
                        (tmp -> get_brother_prev_name()) -> set_brother_next_name(tmp -> get_brother_next_name());

                        if (tmp -> get_brother_next_name())
                            tmp -> get_brother_next_name() -> set_brother_prev_name(tmp -> get_brother_prev_name());


                        if (tmp == l -> get_head()) l -> set_head(tmp -> get_next());
                        if (tmp -> get_next()) tmp -> get_next() -> set_prev(tmp -> get_prev());
                        if (tmp -> get_prev()) tmp -> get_prev() -> set_next(tmp -> get_next());
                        next = tmp -> get_brother_next_name();

                        l -> delete_node_group(tmp);
                        l -> delete_node_phone(tmp);
                        delete tmp;

                        tmp = next;
                    }
                    else tmp = tmp -> get_brother_next_name();
                }
                root = direction_name(root);
            }
        }
        else
            root = direction_name(root);

    }

}
/*-----------------------------------------------------------------------------------------------------------------------------*/
void command :: apply_select(list * l)
{
    list_node * date ;

    if (c_name == COND_NONE && c_phone == COND_NONE && c_group == COND_NONE)
        {if (l -> get_head()) l -> print();}
    else
    {
       if (op != OR)
       {
          // ����� �� ������
          if (c_phone != COND_NONE) search_select_phone(l, l -> get_head());
          else
            if (c_group != COND_NONE) search_select_group(l , l -> get_root_group());
            else
                if (c_name != COND_NONE) search_select_name(l, l -> get_root_name());
        }
       else
          for ( date = l -> get_head() ; date ; date = date -> get_next())
             if (condition(date)) date -> print();
    }
}



void command :: apply_delete(list * l)
{
    list_node * curr, *next = 0 , *prev = 0;
    list_node * date = 0;


    if (c_name == COND_NONE && c_phone == COND_NONE && c_group == COND_NONE)
    {
        for (curr = l -> get_head(); curr; curr = next)
        {
           next = curr -> get_next();
           delete curr;
        }
        l -> set_head(0);
    }
    else
    {
        if (op != OR)
        {
            if (c_name != COND_NONE) search_delete_name(l, l -> get_root_name());
            else
            {
                if (c_group != COND_NONE) search_delete_group(l , l -> get_root_group());
                else
                    if (c_phone != COND_NONE) search_delete_phone(l);
            }
        }
        else
        {
            date = l -> get_head();
            while (date)
            {
               if (condition(date))
               {
                   if (date == l -> get_head())
                   {
                       if (date -> get_next())
                       {
                           l -> set_head(date -> get_next());
                           (l -> get_head()) -> set_prev(0);
                       }
                       delete date;
                       date = l -> get_head();
                   }
                   else
                   {
                      prev = date -> get_prev();
                      if (prev) (prev) -> set_next(date -> get_next());
                      delete date;
                      date = 0;
                      if (prev) date = prev -> get_next();
                   }
               }
               else
                  date = date -> get_next();
            }
        }
    }
}

int command :: apply(list * l)
{

    switch (type)
    {
        case CMD_NONE : return 0;
        case QUIT : return 1;
        case INSERT :
            apply_insert(l);
            break;

        case SELECT :
            apply_select(l);
            break;
        case DELETE :
            apply_delete(l);
            break;
    }
    return 0;
}










