import os
import sys
import zlib
import copy
from collections import deque

#used the Assignment6_Hints powerpoint provided on piazza

class CommitNode:
       def __init__(self, commit_hash):
           """
           :type commit_hash: str
           """
           self.commit_hash = commit_hash
           self.parents = set()
           self.children = set()

#main function
def topo_order_commits():

        #Get git directory (can be helper function)
        git_directory = find_git_dir(os.getcwd())

        #Get list of local branch names (can be helper function)
        branches = get_branch_names(git_directory + "refs/heads")

        #Build the commit graph (can be helper function)
        commit_graph = build_commit_graph(git_directory, branches)

        #Topologically sort the commit graph (can be helper function)
        topo_sort_list = topological_sort(commit_graph)

        #Print the sorted order (can be helper function)
        print_topo_ordered_commits_with_branch_names(commit_graph, topo_sort_list, branches)

if __name__ == '__main__':
    topo_order_commits()

#Get git directory (can be helper function)
def find_git_dir(path): 
    #checks that we are not at the end
    while path != "/": 
        if os.path.exists(path + ".git"): 
            return path + ".git"
        path = os.path.dirname(path)

    if os.path.exists("/.git"): 
        return "/.git"
    else: 
        sys.stderr.write("Not inside a Git repository\n")
        exit (1)

#Get list of local branch names (can be helper function)
#directory is a sub-directory of "git_directory"
def get_branch_names(directory): 
    branch_list = set()

    #loo[ing through the refs/heads folder in the sub-directory (for the first call)
    for i in os.listdir(directory):

        #if it's a directory, then do a recursive call
        if os.path.isdir(i):
            branch_list |= (get_branch_names(i)) # "|=" is like "+=" for a set and is the union 
        else: 
             branch_list.add(i)

    #getting names of branch list
    return branch_list

#pass in a HEAD (file) to find a branch's corresponding commit hash
def git_hash(head): 
    file_descriptor = open(head, 'r')
    first_line = file_descriptor.readline().strip() 
    return first_line #for now, we assume this reads the commit hash

#Build the commit graph (can be helper function)
def build_commit_graph(git_dir, local_branch_heads):
    commit_nodes = {} #Represents your graph
    visited = set()
    stack = local_branch_heads
    while stack:

        #Get the next element from stack, store it in commit_hash, and remove it from stack
        commit_hash = stack.pop()

         #if the commit we’re on is already in visited, then continue
        if commit_hash in visited: 
            continue

        visited.add(commit_hash)

        if commit_hash not in commit_nodes:
            #Create a commit node and store it in the graph for later use
            commit_nodes[commit_hash] = CommitNode.commit_hash
            
        #Using commit_hash, retrieve commit node object from graph
        commit = commit_nodes.commit_hash

        #Find commit_hash in the objects folder, decompress it, and get parent commits
        #commit.parents.update(parent_set) = 
        for p in commit.parents:

            #if p isn’t in visited
            if p not in visited:
               stack.append(p)
            #if p isn’t in commit_nodes (graph)
            if p not in commit_nodes:
                commit_nodes[p] = CommitNode(p)
                #Record that commit_hash is a child of commit node p


    return commit_nodes

'''
DID NOT FINISH: 
'''
#Topologically sort the commit graph (can be helper fnction)
def topological_sort(commit_nodes):
    result = []           # commits we have processed and are now sorted
    no_children = deque() # commits we can process now
    copy_graph = copy.deepcopy(commit_nodes) # Copy graph so we don't erase info
    # If the commit has no children, we can process it
    for commit_hash in copy_graph:
        if len(copy_graph[commit_hash].children) == 0:
            no_children.append(commit_hash)
    #Loop through until all commits are processed
    while len(no_children) > 0:
        commit_hash = no_children.popleft()
        result.append(commit_hash)

        # Now that we are processing commit, remove all connecting edges to parent commits
        # And add parent to processing set if it has no more children after
        for parent_hash in list(copy_graph[commit_hash].parents):
            # Replace with code - Remove parent hash from current commit parents
            # Replace with code - Remove child hash from parent commit children
            # Replace with code - How do we check if parent has no children
                no_children.append(parent_hash)

    # Error check at the end
    if len(result) < len(commit_nodes):
        raise Exception("cycle detected")
    return result


'''
DID NOT FINISH: 
Print the sorted order (can be helper function)
'''
def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')
        branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')



