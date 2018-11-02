if v:lang =~ "utf8$" ||v:lang =~ "UTF-8$"
  set fileencodings=ucs-bom,utf-8,latin1
endif

set nocompatible        " Use Vim defaults
set bs=indent,eol,start " Allow backspacing over everything in insert mode
set viminfo='20,\"50
set history=50
set ruler

if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

filetype plugin indent on

if &term=="xterm"
  set t_Co=8
  set t_Sb=^[[4%dm
  set t_Sf=^[[3%dm
endif

let &guicursor = &guicursor . ",a:blinkon0"

set number
colorscheme elflord
set foldcolumn=2
set shiftwidth=2
set softtabstop=2
set tabstop=2
set expandtab

let g:xml_syntax_folding=1
set foldmethod=syntax

hi FoldColumn ctermbg=black
hi Folded ctermbg=black

" Header include guard
function! s:insert_gates()
  let gatename = substitute(toupper(expand("%:t")), "\\.", "_", "g")
  execute "normal! i#ifndef " . gatename
  execute "normal! o#define " . gatename . " "
  execute "normal! Go#endif // " . gatename
  execute "normal! O"
endfunction

if has("autocmd")
  " Jump to last pos when reopening a file
  autocmd BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
  " Set textwidth for textfiles
  au FileType txt setlocal tw=90 fo+=t pfn=courier:h9
  " Make sure to make make happy
  au FileType make setlocal noexpandtab shiftwidth=8 softtabstop=8 tabstop=8
  " Define comments
  au FileType c,cc,cpp setlocal comments-=:// comments+=f://
  " Add include guard
  autocmd BufNewFile *.{h,hh} call <SID>insert_gates()
endif

