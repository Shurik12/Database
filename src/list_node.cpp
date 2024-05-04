#include "list_node.h"

void list_node :: set_next(list_node* node)
{
	next = node;
}

void list_node :: set_prev(list_node* node)
{
	prev = node;
}
list_node::~list_node() {
			next = 0;
		    prev = 0;

		    next_phone = 0;
			prev_phone = 0;

		    parent_group = 0;
		    left_group = 0;
			right_group = 0;
			brother_prev_group = 0;
			brother_next_group = 0;
			balance_group = 0;

			parent_name = 0;
			left_name = 0;
			right_name = 0;
			brother_prev_name = 0;
			brother_next_name = 0;
			color_name = 0;
		}
/*-------------------------------------------AVL tree phone------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------AVL tree group-------------------------------------------------------------*/

void list_node :: set_parent_group(list_node* node)
{
	parent_group = node;
}


void list_node :: set_left_group(list_node* node)
{
	left_group = node;
}

void list_node :: set_right_group(list_node* node)
{
	right_group = node;
}

void list_node :: set_balance_group(int b)
{
	balance_group = b;
}

void list_node :: set_brother_next_group(list_node* node)
{
	brother_next_group = node;
}

void list_node :: set_brother_prev_group(list_node* node)
{
	brother_prev_group = node;
}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------RB-tree name--------------------------------------------------------------------*/

void list_node :: set_parent_name(list_node* node)
{
	parent_name = node;
}

void list_node :: set_left_name(list_node* node)
{
	left_name = node;
}

void list_node :: set_right_name(list_node* node)
{
	right_name = node;
}

void list_node :: set_color_name(int b)
{
    color_name = b;
}

void list_node :: set_brother_next_name(list_node* node)
{
	brother_next_name = node;
}

void list_node :: set_brother_prev_name(list_node* node)
{
	brother_prev_name = node;
}
