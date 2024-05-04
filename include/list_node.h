#include "record.h"

class list_node : public record
{

	private:
		list_node* next;
		list_node* prev;

		list_node * next_phone;
		list_node * prev_phone;

		list_node * parent_group;
		list_node * left_group;
		list_node * right_group;
		list_node * brother_prev_group;
		list_node * brother_next_group;
		int balance_group;

		list_node * parent_name;
		list_node * left_name;
		list_node * right_name;
		list_node * brother_prev_name;
		list_node * brother_next_name;
		int color_name;

	public:
		list_node(const char* in = 0, int ip = 0, int ig = 0) : record(in, ip, ig) {
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

		list_node(const list_node& a) : record(a) {
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

		~list_node();

		int init(const char* in, int ip, int ig) {
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

			return ((record*)this)->init(in, ip, ig);
		}

		list_node* get_next() {
			return next;
		}

		void set_next(list_node * node);


		list_node* get_prev() {
			return prev;
		}

		void set_prev(list_node * node);
/*---------------------------------------------AVL tree phone-----------------------------------------------------------------*/
		list_node * get_next_phone()
		{
		    return next_phone;
		}

		list_node* get_prev_phone() {
			return prev_phone;
		}

		void set_next_phone(list_node* node)
	    {	next_phone = node; }

		void set_prev_phone(list_node* node)
	    {	prev_phone = node; }

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------AVL tree group-------------------------------------------------------------------*/
        list_node* get_left_group() {
			return left_group;
		}

        void set_left_group(list_node * node);

		list_node* get_right_group() {
			return right_group;
		}

		void set_right_group(list_node * node);

		int get_balance_group() {
			return balance_group;
		}

		list_node* get_brother_next_group() {
			return brother_next_group;
		}

		void set_brother_next_group(list_node * node);

		list_node* get_brother_prev_group() {
			return brother_prev_group;
		}

		void set_brother_prev_group(list_node * node);

		void set_balance_group(int b);

		list_node* get_parent_group() {
			return parent_group;
		}

		void set_parent_group(list_node * node);
/*----------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------RB-tree name---------------------------------------------------------------------*/
        list_node* get_parent_name() {
			return parent_name;
		}

		void set_parent_name(list_node * node);

        list_node* get_left_name() {
			return left_name;
		}

		void set_left_name(list_node * node);

		list_node* get_right_name() {
			return right_name;
		}

		void set_right_name(list_node * node);

		int get_color_name() {
			return color_name;
		}

		list_node* get_brother_next_name() {
			return brother_next_name;
		}

		void set_brother_next_name(list_node * node);

		list_node* get_brother_prev_name() {
			return brother_prev_name;
		}

		void set_brother_prev_name(list_node * node);

		void set_color_name(int b);


};
