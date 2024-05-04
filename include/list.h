#include "list_node.h"

class list {
   private:
      list_node * head;
      list_node * curr;

	  list_node * tail;
      list_node * headhashs[100];
	  list_node * currhashs[100];

      list_node * root_group;
      list_node * root_name;
   public:
	  list();
      ~list();


      void check_tree_phone(list_node * );

      list_node* get_head() {return head;}
      void set_head(list_node* node) {head = node;}
	  list_node* get_curr() {return curr;}
	  list_node * get_headhashs (int i) { return headhashs[i]; }
	  void set_headhashs(list_node* b, int i) { headhashs[i] = b; }
	  list_node * get_currhashs (int i) { return currhashs[i]; }
      void set_currhashs(list_node* b, int i) { currhashs[i] = b; }
	  list_node * get_tail() { return tail; }
	  void set_tail(list_node* b) { tail = b; }
	  void print();
	  int read(std::ifstream& fp);
      int compare_nodes(list_node * , list_node *);

      // AVL tree phone :
	  list_node* get_tail_phone() {return tail;}
      void set_tail_phone(list_node* node) {tail = node;}
	  void length(list_node * , int *  , int i );
	  int add_tail(list_node* b);
	  int del_head();
	  list_node* go_next(list_node* b);
	  int insert_node_phone(list_node *);
	  void delete_node_phone(list_node *  );

	  //AVL tree group :
	  list_node* get_root_group() {return root_group;}
      void set_root_group(list_node* node) {root_group = node;}
	  list_node * rebuild_l1_group(list_node *);
	  list_node * rebuild_l2_group(list_node *);
	  list_node * rebuild_r1_group(list_node *);
	  list_node * rebuild_r2_group(list_node *);
	  list_node * insert_node_group(list_node *);
	  list_node * fix_insert_balance_group(list_node *);
	  void length_phone(list_node * , int *  , int i = 1 );
	  void print_subtree_group(list_node*);
	  void print_tree_group(list_node*, int );
	  void replace_nodes_group(list_node * , list_node *);
	  list_node * delete_node_group(list_node *  );
	  void fix_delete_balance_group(list_node *);


	  //RB - tree name :
	  list_node * get_root_name() {return root_name;}
	  void set_root_name(list_node * node) {root_name = node;}
	  void replace_nodes(list_node * , list_node *);
	  list_node * add_node_name(list_node *);
	  list_node * grandparent(list_node * );
	  list_node * uncle(list_node * );
	  void insert_case1_name(list_node *);
	  void insert_case2_name(list_node *);
	  void insert_case3_name(list_node *);
	  void insert_case4_name(list_node *);
	  void insert_case5_name(list_node *);
	  list_node * rebuild_r1_name(list_node *);
	  list_node * rebuild_l1_name(list_node *);
	  list_node * delete_node_name(list_node *);
	  void delete_case0_name(list_node * );
	  void delete_case1_name(list_node * , list_node * );
	  void delete_case2_name(list_node * );
	  void delete_case3_name(list_node * );
	  void delete_case4_name(list_node * );
	  void delete_case5_name(list_node * );
	  void delete_case6_name(list_node * );

	  void print_subtree_name(list_node*);
	  void print_tree_name(list_node*, int );
};

