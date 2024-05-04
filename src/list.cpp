#include "list.h"

list :: list() {
	int i;
    curr = 0;
    head = 0;
	tail = 0;
	for(i = 0; i < 100; i++)
	{
		headhashs[i]=0;
		currhashs[i]=0;
	}
}

list :: ~list() {
    list_node * next = 0;
    for (curr = head; curr; curr = next) {
        next = curr -> get_next();
        delete curr;
    }
}

void list :: print() {
    list_node *tmp;
    tmp = head;
    while (tmp -> get_next()) {
        tmp -> print();
        tmp = tmp -> get_next();
    }
    tmp -> print();
}

int list :: read(std::ifstream& fp) {
    list_node* tmp = 0;
    list_node* last = 0;

    tmp = new list_node();
    if (tmp -> read(fp)) {
        set_head(tmp);
        set_root_group(tmp);
        set_root_name(tmp);
        tmp -> set_color_name(-1);
    }

    curr = head;
    head -> set_prev(0);

    last = tmp;
    tmp = 0;
    tmp = new list_node();

    while (tmp -> read(fp)) {
        //tmp -> print();
		insert_node_phone(tmp);
		set_tail(tmp);
        last -> set_next(tmp);
        tmp -> set_prev(last);
        last = tmp;
        // root_phone = insert_node_phone(tmp);
        root_group = insert_node_group(tmp);
        tmp -> set_color_name(1);
        root_name = add_node_name(tmp);

        tmp = 0;
        tmp = new list_node();
    }
    last -> set_next(0);
    delete tmp;

    return 1;
}

/* --------------------------------------AVL tree phone------------------------------------------------------------*/

int list::add_tail(list_node *q) {
	if (head != 0) {
		tail->set_next(q);
		q->set_prev(tail);
		tail=tail->get_next();
		curr=get_tail_phone();
	} else {
		head=q;
		tail=q;
		curr=q;
	}
	return 0;
}

int list::del_head() {
	list_node *q;
	if (head!=0) {
		q=head;
		head=head->get_next();
		curr=get_head();
		delete q;
	} else {
        printf("error 208");
    }
	return 0;
}

list_node* list::go_next(list_node * h) {
	if(h->get_next()!=0) h=h->get_next();
	else printf("error");
	return h;
}

void list::delete_node_phone(list_node * q) {
	int k;
	k = q->get_phone();
	k = (k - k % 100000) / 100000;
	if (q == this->get_headhashs(k)) {
		this->set_headhashs(q->get_next_phone(), k);
		if (q -> get_next_phone()) q -> get_next_phone() -> set_prev_phone(0);
	} else if (q->get_next_phone()==0) {
		this -> set_currhashs(q -> get_prev_phone(), k);
		if (q -> get_prev_phone())
            (q -> get_prev_phone())->set_next_phone(0);
	} else {
		q->get_prev_phone()->set_next_phone(q->get_next_phone());
		q->get_next_phone()->set_prev_phone(q->get_prev_phone());
	}
	if (q == this->get_head()) {//���������� �� ������
		this->set_head(q->get_next());
		q->get_next()->set_prev(0);
	} else if (q == this->get_tail()) {
		this->set_tail(q->get_prev());
		q->get_prev()->set_next(0);
	} else {
		if (q->get_prev()) {
            (q->get_prev())->set_next(q->get_next());
        } 
        if (q->get_next()) {
            (q->get_next())->set_prev(q->get_prev());
        }
	}
}



int list::insert_node_phone(list_node *b) {
	int i, k=b->get_phone();
	k = (k - k % 100000) / 100000;
	for (i=0; i<100; i++) {
		if (k == i) {
			if (headhashs[i] == 0) {
				headhashs[i]=b;
				currhashs[i]=b;
				currhashs[i]->set_next_phone(0);
				currhashs[i]->set_prev_phone(0);
			}
			else {
				currhashs[i] -> set_next_phone(b);
				b -> set_prev_phone(currhashs[i]);
				currhashs[i] = b;
				currhashs[i] -> set_next_phone(0);
			}
			break;
		}
	}
	return 0;
}


/*----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------AVL tree group-----------------------------------------------------------------*/
void list:: print_subtree_group(list_node* tmp)
{
    if (tmp != 0)
    {
        tmp -> print();

        print_subtree_group(tmp -> get_left_group());
        print_subtree_group(tmp -> get_right_group());
        print_subtree_group(tmp -> get_brother_next_group());
    }

}


void list:: print_tree_group(list_node* tmp, int level = 0)
{
    int i ;

    if (tmp != 0)
    {

        for (i = 0; i < level ; i++)
            printf(" ");

        tmp -> print();

        print_tree_group(tmp -> get_left_group(), level + 1);
        print_tree_group(tmp -> get_right_group(), level + 1);
        print_tree_group(tmp -> get_brother_next_group(), level);
    }

}


list_node * list :: rebuild_r1_group(list_node * a)
{
    list_node * b;
    list_node * p_a = a -> get_parent_group(); //

    b = a -> get_left_group();

    if (a == root_group) set_root_group(b);

    a -> set_left_group(b -> get_right_group());
    if (b -> get_right_group()) b -> get_right_group() -> set_parent_group(a); //

    b -> set_right_group(a);
    b -> set_parent_group(a -> get_parent_group()); //
    if (p_a) //
    {
        if (p_a -> get_left_group() == a)
            p_a -> set_left_group(b);
        else p_a -> set_right_group(b);
    }

    a -> set_parent_group(b); //
    b -> set_parent_group(p_a);

    if (b -> get_balance_group())
    {
        a -> set_balance_group(0);
        b -> set_balance_group(0);
    }
    else
    {
        a -> set_balance_group(1);
        b -> set_balance_group(-1);
    }

    return b;
}

list_node * list :: rebuild_r2_group(list_node * a)
{
    list_node * b , * c;
    list_node * p_a = a -> get_parent_group(); //

    b = a -> get_left_group();
    c = b -> get_right_group();

    if (a == root_group) set_root_group(c);

    a -> set_left_group(c -> get_right_group());
    if (c -> get_right_group()) c -> get_right_group() -> set_parent_group(a); //
    b -> set_right_group(c -> get_left_group());
    if (c -> get_left_group()) c -> get_left_group() -> set_parent_group(b); //

    c -> set_left_group(b);
    c -> set_right_group(a);

    if (p_a) //
    {
        if (p_a -> get_left_group() == a)
            p_a -> set_left_group(c);
        else p_a -> set_right_group(c);
    }

    b -> set_parent_group(c); //
    a -> set_parent_group(c); //
    c -> set_parent_group(p_a);

    switch (c -> get_balance_group())
    {
        case 0 :
            a -> set_balance_group(0);
            b -> set_balance_group(0);
            break;

        case -1 :
            a -> set_balance_group(-1);
            b -> set_balance_group(0);
            break;

        case 1 :
            a -> set_balance_group(0);
            b -> set_balance_group(1);

    }

    c -> set_balance_group(0);

    return c;
}

list_node * list :: rebuild_l1_group(list_node * a)
{
    list_node * b;
    list_node * p_a = a -> get_parent_group();

    b = a -> get_right_group();

    if (a == root_group) set_root_group(b);

    a -> set_right_group(b -> get_left_group());
    if (b -> get_left_group()) b -> get_left_group() -> set_parent_group(a); //

    b -> set_left_group(a);
    a -> set_parent_group(b);

    if (p_a) //
    {
        if (p_a -> get_left_group() == a)
            p_a -> set_left_group(b);
        else p_a -> set_right_group(b);
    }

    b -> set_parent_group(p_a); //

    if (b -> get_balance_group())
    {
        a -> set_balance_group(0);
        b -> set_balance_group(0);
    }
    else
    {
        a -> set_balance_group(-1);
        b -> set_balance_group(1);
    }
    return b;
}

list_node * list :: rebuild_l2_group(list_node * a)
{
    list_node * b , * c;
    list_node * p_a = a -> get_parent_group();

    b = a -> get_right_group();
    c = b -> get_left_group();

    if (a == root_group) set_root_group(c);

    b -> set_left_group(c -> get_right_group());
    if (c -> get_right_group()) c -> get_right_group() -> set_parent_group(b);

    a -> set_right_group(c -> get_left_group());
    if (c -> get_left_group()) c -> get_left_group() -> set_parent_group(a);

    c -> set_left_group(a);
    c -> set_right_group(b);

    if (p_a) //
    {
        if (p_a -> get_left_group() == a)
            p_a -> set_left_group(c);
        else p_a -> set_right_group(c);
    }

    b -> set_parent_group(c); //
    a -> set_parent_group(c); //
    c -> set_parent_group(p_a);

    switch (c -> get_balance_group())
    {
    case 0 :
        a -> set_balance_group(0);
        b -> set_balance_group(0);
        break;

    case 1 :
        a -> set_balance_group(0);
        b -> set_balance_group(-1);
        break;

    case -1 :
        a -> set_balance_group(1);
        b -> set_balance_group(0);
    }

    c -> set_balance_group(0);
    return c;
}

void list :: length(list_node *node , int * l , int i )
{
    if (node != 0)
    {
        //printf("%d\n", *l);
        //node -> print();
        length(node -> get_left_group() , l , i+1);
        length(node -> get_right_group() , l , i+1);
    }
    if (node == 0)
       if ((*l) <= i-1)
       {
           (*l) = i-1;

       }
}



list_node * list :: fix_insert_balance_group(list_node * node)
{
    int l = 0;
    int r = 0;
    int b_node = 0;
    int b_child = 0;
    int i = 1;

    list_node * left = 0, * right = 0;

    if (node != 0)
    {
        right = node -> get_right_group() ;
        left = node -> get_left_group();
        length(node -> get_left_group(), &l , i);
        i = 1;
        length(node -> get_right_group() , &r , i);
        b_node = l - r;

        switch (b_node)
        {
           case 0 : break;
           case -2 :

                l = 0;
                r = 0;
                length(right -> get_left_group(), &l , i);
                length(right -> get_right_group(), &r , i);
                b_child = l - r;
                switch(b_child)
                {
                    case  -1 :
                        node = rebuild_l1_group(node);
                        //node = fix_balance(node);
                        break;

                    case 0 :
                        node = rebuild_l1_group(node);
                        node = fix_insert_balance_group(node);
                        break;

                    case 1 :
                        node = rebuild_l2_group(node);
                        //node = fix_balance(node);
                        break;
                }
                break;
           case 2 :
                l = 0;
                r = 0;
                length(left -> get_left_group(), &l , i);
                length(left -> get_right_group(), &r , i);
                b_child = l - r;
                switch(b_child)
                {
                    case  1 :
                        node = rebuild_r1_group(node);
                        //node = fix_balance(node);
                        break;

                    case 0 :
                        node = rebuild_r1_group(node);
                        node = fix_insert_balance_group(node);
                        break;

                    case -1 :
                        node = rebuild_r2_group(node);
                        //node = fix_balance(node);
                        break;
                }
                break;

           case 1 :
                node = fix_insert_balance_group(node -> get_parent_group());
                break;

           case -1 :
                node = fix_insert_balance_group(node -> get_parent_group());
                break;
        }
    }
    return node;
}

list_node * list :: insert_node_group(list_node * node)
{
    list_node * root = root_group;

    while (root)
    {
        if (node -> get_group() == root -> get_group())
        {
            node -> set_parent_group(root -> get_parent_group());
            node -> set_balance_group(root -> get_balance_group());
            if (root -> get_brother_next_group())
                root -> get_brother_next_group() -> set_brother_prev_group(node);
            node -> set_brother_next_group(root -> get_brother_next_group());
            node -> set_brother_prev_group(root);
            root -> set_brother_next_group(node);

            break;
        }
        else
        {
            if (node -> get_group() > root -> get_group())
            {
                if (root -> get_right_group())
                {
                    root -> set_balance_group(root -> get_balance_group() - 1);
                    root = root -> get_right_group();
                }
                else
                {
                    root -> set_right_group(node);
                    node -> set_parent_group(root);
                    fix_insert_balance_group(node -> get_parent_group());
                    break;
                }
            }
            else
            {
                if (node -> get_group() < root -> get_group())
                {
                    if (root -> get_left_group())
                    {
                        root -> set_balance_group(root -> get_balance_group() + 1);
                        root = root -> get_left_group();
                    }
                    else
                    {
                        root -> set_left_group(node);
                        node -> set_parent_group(root);
                        fix_insert_balance_group(node -> get_parent_group());
                        break;
                    }
                }
            }
        }
    }

    return root_group;
}

void list :: replace_nodes_group(list_node * a, list_node * b)
{
    list_node * p_a = a -> get_parent_group();
    list_node * p_b = b -> get_parent_group();
    list_node * l_a = a -> get_left_group();
    list_node * r_a = a -> get_right_group();
    list_node * l_b = b -> get_left_group();
    list_node * r_b = b -> get_right_group();


    if (a != b -> get_parent_group())
    {
        a -> set_parent_group(p_b);
        b -> set_parent_group(p_a);
        a -> set_left_group(l_b);
        a -> set_right_group(r_b);
        b -> set_left_group(l_a);
        b -> set_right_group(r_a);
        if (l_a) l_a -> set_parent_group(b);
        if (r_a) r_a -> set_parent_group(b);
        if (l_b) l_b -> set_parent_group(a);
        if (r_b) r_b -> set_parent_group(a);

        if (p_a)
        {
            if (a == p_a -> get_left_group()) p_a -> set_left_group(b);
            else p_a -> set_right_group(b);
        }
        if (p_b)
        {
            if (b == p_b -> get_left_group()) p_b -> set_left_group(a);
            else p_b -> set_right_group(a);
        }
    }
    else
    {
        a -> set_parent_group(b);
        b -> set_parent_group(p_a);
        if ( b == a -> get_left_group())
        {
            b -> set_left_group(a);
            b -> set_right_group(r_a);
            if (r_a) r_a -> set_parent_group(b);
        }
        else
        {
            b -> set_right_group(a);
            b -> set_left_group(l_a);
            if (l_a) l_a -> set_parent_group(b);
        }
        if (p_a)
        {
            if (a == p_a -> get_left_group()) p_a -> set_left_group(b);
            else p_a -> set_right_group(b);
        }
        if (l_b) l_b -> set_parent_group(a);
        if (r_b) r_b -> set_parent_group(a);
        a -> set_right_group(r_b);
        a -> set_left_group(l_b);

    }
}

void list :: fix_delete_balance_group(list_node * node)
{
    int l = 0;
    int r = 0;
    int b_node = 0;
    int b_child = 0;
    int i = 1;

    list_node * right = 0 , * left = 0;

    if (node != 0)
    {
        right = node -> get_right_group() ;
        left = node -> get_left_group();
        length(node -> get_left_group(), &l , i);
        i = 1;
        length(node -> get_right_group() , &r , i);
        b_node = l - r;

        switch (b_node)
        {
           case 0 :
               fix_delete_balance_group(node -> get_parent_group());
               break;
           case -2 :

                l = 0;
                r = 0;
                length(right -> get_left_group(), &l , i);
                length(right -> get_right_group(), &r , i);
                b_child = l - r;
                switch(b_child)
                {
                    case  -1 :
                        node = rebuild_l1_group(node);
                        fix_delete_balance_group(node);
                        break;

                    case 0 :
                        node = rebuild_l1_group(node);
                        fix_delete_balance_group(node);
                        break;

                    case 1 :
                        node = rebuild_l2_group(node);
                        fix_delete_balance_group(node);
                        break;
                }
                break;
           case 2 :
                l = 0;
                r = 0;
                length(left -> get_left_group(), &l , i);
                length(left -> get_right_group(), &r , i);
                b_child = l - r;
                switch(b_child)
                {
                    case  1 :
                        node = rebuild_r1_group(node);
                        fix_delete_balance_group(node);
                        break;

                    case 0 :
                        node = rebuild_r1_group(node);
                        fix_delete_balance_group(node);
                        break;

                    case -1 :
                        node = rebuild_r2_group(node);
                        fix_delete_balance_group(node);
                        break;
                }
                break;

           case 1 :
                //fix_delete_balance_group(node -> get_parent_group());
                break;

           case -1 :
                //fix_delete_balance_group(node -> get_parent_group());
                break;
        }
    }

}


list_node * list :: delete_node_group(list_node * root)
{
    list_node * pos = root;
    list_node * next = root -> get_brother_next_group();
    list_node * prev = root -> get_brother_prev_group();
    list_node * parent = root -> get_parent_group();
    list_node * left = root -> get_left_group();
    list_node * right = root -> get_right_group();


    if (prev != 0)
    {
        (prev) -> set_brother_next_group(next);

        if (next)
            next -> set_brother_prev_group(prev);

        return 0;
    }

    if (next)
    {
        if (root == root_group) set_root_group(next);
        (next) -> set_brother_prev_group(0);
        if (parent)
        {
            if (parent -> get_left_group() == root)
			{
                parent -> set_left_group(next);
				next -> set_parent_group(parent);
			}
			else 
			{
				parent -> set_right_group(next); 
				next -> set_parent_group(parent);
			}
        }

        next -> set_left_group(left);
        if (left) left -> set_parent_group(next);

        next -> set_right_group(right);
        if (right) right -> set_parent_group(next);

        return 0;
    }


    if (root -> get_left_group())
    {
        for (pos = root -> get_left_group(); pos -> get_right_group() ; )
            pos = pos -> get_right_group();

        replace_nodes_group(root , pos);

        if (root != pos -> get_left_group())
        {
            if (root -> get_parent_group())
                (root -> get_parent_group()) -> set_right_group(root -> get_left_group());

            if (root -> get_left_group())
                root -> get_left_group() -> set_parent_group(root -> get_parent_group());
        }
        else
        {
            (pos) -> set_left_group(root -> get_left_group());
            if (root -> get_left_group())
                root -> get_left_group() -> set_parent_group(pos);
        }
    }
    else
    {
        if (root -> get_right_group())
        {
            pos = root -> get_right_group();

            replace_nodes_group(root, pos);

            if (root != pos -> get_right_group())
            {
                if (root -> get_parent_group())
                    (root -> get_parent_group()) -> set_left_group(root -> get_right_group());
                if (root -> get_right_group())
                    root -> get_right_group() -> set_parent_group(root -> get_parent_group());
            }
            else
            {
                (pos) -> set_right_group(root -> get_right_group());
                if (root -> get_right_group())
                    root -> get_right_group() -> set_parent_group(pos);
            }
        }
    }

    if (pos != root)
    {
        if (root == root_group) set_root_group(pos);

        fix_delete_balance_group(root -> get_parent_group());

        //delete root;
    }
    else
    {
        if (root == root_group) set_root_group(0);
        else
        {
            if (root == root -> get_parent_group() -> get_left_group())
                root -> get_parent_group() -> set_left_group(0);
            else
                root -> get_parent_group() -> set_right_group(0);
        }

        fix_delete_balance_group(root -> get_parent_group());

       // delete root;
        pos = 0;
        root = 0;
    }

    return root_group;
}

/*-----------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------RB-tree name-----------------------------------------------------------------*/
void list:: print_subtree_name(list_node* tmp)
{
    if (tmp != 0)
    {
        tmp -> print();
        print_subtree_name(tmp -> get_left_name());
        print_subtree_name(tmp -> get_right_name());
        print_subtree_name(tmp -> get_brother_next_name());
    }

}

void list:: print_tree_name(list_node* tmp, int level = 0)
{
    int i ;
    //HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
    if (tmp != 0)
    {
        //printf("%d\n",tmp -> get_color_name());
        for (i = 0; i < level ; i++)
            printf(" ");

        /*switch (tmp -> get_color_name())
        {
        case 1 :
            SetConsoleTextAttribute(hOUTPUT,FOREGROUND_RED | FOREGROUND_INTENSITY);
            break;
        case -1 :
            SetConsoleTextAttribute(hOUTPUT,FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
        }*/

        tmp -> print();
        //SetConsoleTextAttribute(hOUTPUT,FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        print_tree_name(tmp -> get_left_name(), level + 1);
        print_tree_name(tmp -> get_right_name(), level + 1);
        print_tree_name(tmp -> get_brother_next_name(), level);
    }

}


list_node * list :: grandparent(list_node * node)
{
    if ((node != 0) && (node -> get_parent_name() != 0))
        return (node -> get_parent_name()) -> get_parent_name();
    else
        return 0;
}

list_node * list :: uncle(list_node * node)
{
    list_node *g = grandparent(node);
    if (g == 0)
        return 0;
    if (node -> get_parent_name() == g -> get_left_name())
        return g -> get_right_name();
    else
        return g -> get_left_name();
}


void list :: insert_case1_name(list_node * node)
{
    //node -> print();
    if (node -> get_parent_name() == 0)
        node -> set_color_name(-1);
    else
        insert_case2_name(node);
}

void list :: insert_case2_name(list_node * node)
{
    //node -> print();
    if ((node -> get_parent_name()) -> get_color_name() == -1)
        return;
    else
        insert_case3_name(node);
}

void list :: insert_case3_name(list_node * node)
{
    list_node * u = uncle(node), * g;


    if ((u != 0) && (u -> get_color_name() == 1) && ((node -> get_parent_name()) -> get_color_name() == 1))
    {

        (node-> get_parent_name()) -> set_color_name(-1);
        u -> set_color_name(-1);
        g = grandparent(node);
        g -> set_color_name(1);
        insert_case1_name(g);
    }
    else insert_case4_name(node);
}

list_node * list :: rebuild_l1_name(list_node * a)
{
    list_node * b;
    list_node * p_a = a -> get_parent_name();

    b = a -> get_right_name();
    if (a == root_name) set_root_name(b);
    a -> set_right_name(b -> get_left_name());
    if (b -> get_left_name()) (b -> get_left_name()) -> set_parent_name(a);
    b -> set_left_name(a);
    a -> set_parent_name(b);
    b -> set_parent_name(p_a);
    if (p_a)
    {
        if (a == p_a -> get_right_name()) p_a -> set_right_name(b);
        else p_a -> set_left_name(b);
    }

    return a;
}

list_node * list :: rebuild_r1_name(list_node * a)
{

    list_node * b;
    list_node * p_a = a -> get_parent_name();

    b = a -> get_left_name();
    if (a == root_name) set_root_name(b);
    a -> set_left_name(b -> get_right_name());
    if (b -> get_right_name()) (b -> get_right_name()) -> set_parent_name(a);

    b -> set_right_name(a);
    a -> set_parent_name(b);
    b -> set_parent_name(p_a);
    if (p_a)
    {
        if (a == p_a -> get_left_name()) p_a -> set_left_name(b);
        else p_a -> set_right_name(b);
    }

    return a;
}

void list :: insert_case4_name(list_node * node)
{
    list_node * g = grandparent(node);

    if ((node  == (node -> get_parent_name()) -> get_right_name()) &&
            (g -> get_left_name())&&(node -> get_parent_name() == g -> get_left_name()))

        node = rebuild_l1_name(node -> get_parent_name());
    else
        if ((node == (node -> get_parent_name()) -> get_left_name()) &&
             (g -> get_right_name()) && (node -> get_parent_name() == g -> get_right_name()))

        node = rebuild_r1_name(node -> get_parent_name());

    insert_case5_name(node);
}

void list :: insert_case5_name(list_node * node)
{
    list_node *g = grandparent(node);

    if (node -> get_parent_name()) (node -> get_parent_name()) -> set_color_name(-1);
    if (g) g -> set_color_name(1);
    if ((node == (node -> get_parent_name()) -> get_left_name()) && (node -> get_parent_name() == g -> get_left_name()))
    {
        rebuild_r1_name(g);
    }
    else
    {
        rebuild_l1_name(g);
    }
}

list_node * list :: add_node_name(list_node * node)
{
    list_node * root = root_name;

    while (root)
    {
        if (strcmp(node -> get_name() , root -> get_name()) > 0)
        {
            if (root -> get_right_name()) root = root -> get_right_name();
            else
            {
                root -> set_right_name(node);
                node -> set_parent_name(root);
                insert_case1_name(node);
                break;
            }
        }
        if (strcmp(node -> get_name() , root -> get_name()) < 0)
        {
            if (root -> get_left_name()) root = root -> get_left_name();
            else
            {
                root -> set_left_name(node);
                node -> set_parent_name(root);
                insert_case1_name(node);
                break;
            }
        }
        if (strcmp(root -> get_name(), node -> get_name()) == 0)
        {
            //printf("!!!\n");
            node -> set_color_name(root -> get_color_name());
            node -> set_parent_name(root -> get_parent_name());

            if (root -> get_brother_next_name())
                (root -> get_brother_next_name()) -> set_brother_prev_name(node);

            node -> set_brother_next_name(root -> get_brother_next_name());
            root -> set_brother_next_name(node);
            node -> set_brother_prev_name(root);

            break;
        }

    }

    return root_name;
}


void list :: replace_nodes(list_node * a, list_node * b)
{
    list_node * p_a = a -> get_parent_name();
    list_node * p_b = b -> get_parent_name();
    list_node * l_a = a -> get_left_name();
    list_node * r_a = a -> get_right_name();
    list_node * l_b = b -> get_left_name();
    list_node * r_b = b -> get_right_name();


    if (a != b -> get_parent_name())
    {
        a -> set_parent_name(p_b);
        b -> set_parent_name(p_a);
        a -> set_left_name(l_b);
        a -> set_right_name(r_b);
        b -> set_left_name(l_a);
        b -> set_right_name(r_a);
        if (l_a) l_a -> set_parent_name(b);
        if (r_a) r_a -> set_parent_name(b);
        if (l_b) l_b -> set_parent_name(a);
        if (r_b) r_b -> set_parent_name(a);

        if (p_a)
        {
            if (a == p_a -> get_left_name()) p_a -> set_left_name(b);
            else p_a -> set_right_name(b);
        }
        if (p_b)
        {
            if (b == p_b -> get_left_name()) p_b -> set_left_name(a);
            else p_b -> set_right_name(a);
        }
    }
    else
    {
        a -> set_parent_name(b);
        b -> set_parent_name(p_a);
        if ( b == a -> get_left_name())
        {
            b -> set_left_name(a);
            b -> set_right_name(r_a);
            if (r_a) r_a -> set_parent_name(b);
        }
        else
        {
            b -> set_right_name(a);
            b -> set_left_name(l_a);
            if (l_a) l_a -> set_parent_name(b);
        }
        if (p_a)
        {
            if (a == p_a -> get_left_name()) p_a -> set_left_name(b);
            else p_a -> set_right_name(b);
        }
        if (l_b) l_b -> set_parent_name(a);
        if (r_b) r_b -> set_parent_name(a);
        a -> set_right_name(r_b);
        a -> set_left_name(l_b);

    }
}

list_node * list :: delete_node_name(list_node * root)
{
    list_node * pos = root;
    //list_node * child;
    list_node * next = root -> get_brother_next_name();
    list_node * prev = root -> get_brother_prev_name();
    list_node * parent = root -> get_parent_name();
    list_node * left = root -> get_left_name();
    list_node * right = root -> get_right_name();
    list_node * node = 0;
    list_node * brother;
    //int i = 0;

    if (prev != 0)
    {
        prev -> set_brother_next_name(next);

        if (next)
            next -> set_brother_prev_name(prev);

        return 0;
    }

    if (next)
    {
        if (root == root_name) set_root_name(next);
        (next) -> set_brother_prev_name(0);
        if (parent)
        {
            if (parent -> get_left_name() == root)
                parent -> set_left_name(next);
            else parent -> set_right_name(next);
        }

        next -> set_parent_name(parent);

        next -> set_left_name(left);
        if (left) left -> set_parent_name(next);

        next -> set_right_name(right);
        if (right) right -> set_parent_name(next);
        return 0;
    }
    if ((root -> get_left_name() != 0))
    {
        for (pos = root -> get_left_name() ; pos -> get_right_name() ; )
            pos = pos -> get_right_name();

        replace_nodes(root, pos);

        if (root != pos -> get_left_name())
        {
            (root -> get_parent_name()) -> set_right_name(root -> get_left_name());
            if (root -> get_left_name())
                root -> get_left_name() -> set_parent_name(root -> get_parent_name());
        }
        else
        {
            (pos) -> set_left_name(root -> get_left_name());
            if (root -> get_left_name())
                root -> get_left_name() -> set_parent_name(pos);
        }
        node = root -> get_left_name();
        parent = root -> get_parent_name();
        //node -> set_color_name(-1);
    }
    else
    {
        if (root -> get_right_name() != 0)
        {
            for (pos = root -> get_right_name() ; pos -> get_left_name(); )
                pos = pos -> get_left_name();

            replace_nodes(root, pos);

            if (root != pos -> get_right_name())
            {
                (root -> get_parent_name()) -> set_left_name(root -> get_right_name());
                if (root -> get_right_name())
                    root -> get_right_name() -> set_parent_name(root -> get_parent_name());
            }
            else
            {
                (pos) -> set_right_name(root -> get_right_name());
                if (root -> get_right_name())
                    root -> get_right_name() -> set_parent_name(pos);
            }
            node = root -> get_right_name();
            parent = root -> get_parent_name();
            //node -> set_color_name(-1);
        }
        else
        {
            parent = root -> get_parent_name();
            if (parent)
            {
                if (parent -> get_left_name() == root)
                   parent -> set_left_name(0);
                else parent -> set_right_name(0);
                node = parent;
                parent = node -> get_parent_name();
            }
        }
    }
    //node -> print();
    //if (pos != root)
    {
        if (root == root_name) set_root_name(pos);


        if (root -> get_color_name() == -1)
        {
            if (pos -> get_color_name() == 1)
                pos -> set_color_name(-1);
            else
            {
                if (node == 0 || node -> get_color_name() == -1)
                    while (node == 0 || (node != root_name && node -> get_color_name() == -1))
                    {
                        //parent = node -> get_parent_name();
                        if (parent -> get_left_name() == 0 || node == parent -> get_left_name())
                        {
                            brother = parent -> get_right_name();

                            switch (brother -> get_color_name())
                            {
                                case 1 :
                                    parent -> set_color_name(1);
                                    brother -> set_color_name(-1);
                                    node = rebuild_l1_name(parent);
                                    node =  parent;
                                    break;
                                case -1 :
                                    if ((brother -> get_left_name() == 0 || brother -> get_left_name() -> get_color_name() == -1)&&
                                        (brother -> get_right_name() == 0 || brother -> get_right_name() -> get_color_name() == -1))
                                        brother -> set_color_name(-1);
                                    else
                                    {
                                        if ((brother -> get_left_name() != 0 && brother -> get_left_name() -> get_color_name() == 1)&&
                                        (brother -> get_right_name() == 0 || brother -> get_right_name() -> get_color_name() == -1))
                                        {
                                            //node -> print();
                                            brother -> set_color_name(-1);
                                            brother -> get_left_name() -> set_color_name(-1);
                                            rebuild_r1_name(brother);
                                            node = parent -> get_parent_name();
                                            parent = node -> get_parent_name();
                                        }
                                        else
                                        {
                                            if (brother -> get_right_name() -> get_color_name() == 1)
                                            {
                                                brother -> set_color_name(parent -> get_color_name());
                                                brother -> get_right_name() -> set_color_name(-1);
                                                parent -> set_color_name(-1);
                                                node = rebuild_l1_name(parent);
                                                parent = node -> get_parent_name();
                                            }
                                        }
                                    }

                            }
                        }
                        else
                        if (parent -> get_right_name() == 0 || node == parent -> get_right_name())
                        {
                            brother = parent -> get_left_name();
                            switch (brother -> get_color_name())
                            {
                                case 1 :
                                    parent -> set_color_name(1);
                                    brother -> set_color_name(-1);
                                    node = rebuild_r1_name(parent);
                                    node =  parent;
                                    break;
                                case -1 :
                                    if ((brother -> get_right_name() == 0 || brother -> get_right_name() -> get_color_name() == -1)&&
                                        (brother -> get_left_name() == 0 || brother -> get_left_name() -> get_color_name() == -1))
                                        brother -> set_color_name(-1);
                                    else
                                    {
                                        if ((brother -> get_right_name() != 0 &&
                                             brother -> get_right_name() -> get_color_name() == 1)&&
                                            (brother -> get_left_name() == 0 ||
                                             brother -> get_left_name() -> get_color_name() == -1))
                                        {
                                            //node -> print();
                                            brother -> set_color_name(-1);
                                            brother -> get_right_name() -> set_color_name(-1);
                                            rebuild_l1_name(brother);
                                            node = parent -> get_parent_name();
                                            parent = node -> get_parent_name();
                                        }
                                        else
                                        {
                                            if (brother -> get_left_name() -> get_color_name() == 1)
                                            {
                                                brother -> set_color_name(parent -> get_color_name());
                                                brother -> get_left_name() -> set_color_name(-1);
                                                parent -> set_color_name(-1);
                                                node = rebuild_r1_name(parent);
                                                parent = node -> get_parent_name();
                                            }
                                        }
                                    }

                            }
                        }
                        /*printf("&\n");
                        print_tree_name(root_name , i);
                        printf("&\n");*/
                        node = parent;
                        parent = parent -> get_parent_name();
                        if (parent)
                        {
                            if (node == parent -> get_left_name())
                                brother = parent -> get_right_name();
                            else brother = parent -> get_left_name();
                        }
                    }
                else node -> set_color_name(-1);

            }
        }

        //delete root;
    }


    //return pos;
    return root_name;
}


