if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
inoremap <C-Space> 
imap <Nul> <C-Space>
inoremap <expr> <Up> pumvisible() ? "\" : "\<Up>"
inoremap <expr> <S-Tab> pumvisible() ? "\" : "\<S-Tab>"
inoremap <expr> <Down> pumvisible() ? "\" : "\<Down>"
inoremap <silent> <C-Tab> =UltiSnips#ListSnippets()
inoremap <silent> <Plug>NERDCommenterInsert  <BS>:call NERDComment('i', "insert")
inoremap <silent> <Plug>delimitMateMRightMouse =delimitMate#Finish(1)<RightMouse>
inoremap <silent> <Plug>delimitMateMLeftMouse =delimitMate#Finish(1)<LeftMouse>
inoremap <silent> <Plug>delimitMateDel =delimitMate#Del()
inoremap <silent> <Plug>delimitMateS-Tab =delimitMate#JumpAny("\<S-Tab>")
inoremap <silent> <Plug>delimitMateSpace =delimitMate#ExpandSpace()
inoremap <silent> <Plug>delimitMateCR =delimitMate#ExpandReturn()
inoremap <silent> <expr> <Plug>delimitMateS-BS delimitMate#WithinEmptyPair() ? "\=delimitMate#Del()\" : "\<S-BS>"
inoremap <silent> <Plug>delimitMateBS =delimitMate#BS()
inoremap <silent> <Plug>delimitMate` =delimitMate#QuoteDelim("\`")
inoremap <silent> <Plug>delimitMate' =delimitMate#QuoteDelim("\'")
inoremap <silent> <Plug>delimitMate" =delimitMate#QuoteDelim("\"")
inoremap <silent> <Plug>delimitMate] =delimitMate#JumpOut("\]")
inoremap <silent> <Plug>delimitMate} =delimitMate#JumpOut("\}")
inoremap <silent> <Plug>delimitMate) =delimitMate#JumpOut("\)")
inoremap <silent> <Plug>delimitMate[ [=delimitMate#ParenDelim("]")
inoremap <silent> <Plug>delimitMate{ {=delimitMate#ParenDelim("}")
inoremap <silent> <Plug>delimitMate( (=delimitMate#ParenDelim(")")
snoremap <silent>  c
nmap  :call SearchWord()
xnoremap <silent>  :call multiple_cursors#new("v", 0)
nnoremap <silent>  :call multiple_cursors#new("n", 1)
snoremap  "_c
nmap   @:
nnoremap 'd :YcmShowDetailedDiagnostic
xnoremap <silent> '	 :call UltiSnips#SaveLastVisualSelection()gvs
snoremap <silent> '	 :call UltiSnips#ExpandSnippetOrJump()
nnoremap <silent> 'R :call ri#OpenSearchPrompt(1)
nnoremap <silent> 'r :call ri#OpenSearchPrompt(0)
nmap 'ca <Plug>NERDCommenterAltDelims
xmap 'cu <Plug>NERDCommenterUncomment
nmap 'cu <Plug>NERDCommenterUncomment
xmap 'cb <Plug>NERDCommenterAlignBoth
nmap 'cb <Plug>NERDCommenterAlignBoth
xmap 'cl <Plug>NERDCommenterAlignLeft
nmap 'cl <Plug>NERDCommenterAlignLeft
nmap 'cA <Plug>NERDCommenterAppend
xmap 'cy <Plug>NERDCommenterYank
nmap 'cy <Plug>NERDCommenterYank
xmap 'cs <Plug>NERDCommenterSexy
nmap 'cs <Plug>NERDCommenterSexy
xmap 'ci <Plug>NERDCommenterInvert
nmap 'ci <Plug>NERDCommenterInvert
nmap 'c$ <Plug>NERDCommenterToEOL
xmap 'cn <Plug>NERDCommenterNested
nmap 'cn <Plug>NERDCommenterNested
xmap 'cm <Plug>NERDCommenterMinimal
nmap 'cm <Plug>NERDCommenterMinimal
xmap 'c  <Plug>NERDCommenterToggle
nmap 'c  <Plug>NERDCommenterToggle
xmap 'cc <Plug>NERDCommenterComment
nmap 'cc <Plug>NERDCommenterComment
vnoremap <silent> ''w :call EasyMotion#WB(1, 0)
onoremap <silent> ''w :call EasyMotion#WB(0, 0)
nnoremap <silent> ''w :call EasyMotion#WB(0, 0)
vnoremap <silent> ''t :call EasyMotion#T(1, 0)
onoremap <silent> ''t :call EasyMotion#T(0, 0)
nnoremap <silent> ''t :call EasyMotion#T(0, 0)
vnoremap <silent> ''n :call EasyMotion#Search(1, 0)
onoremap <silent> ''n :call EasyMotion#Search(0, 0)
nnoremap <silent> ''n :call EasyMotion#Search(0, 0)
vnoremap <silent> ''k :call EasyMotion#JK(1, 1)
onoremap <silent> ''k :call EasyMotion#JK(0, 1)
nnoremap <silent> ''k :call EasyMotion#JK(0, 1)
vnoremap <silent> ''j :call EasyMotion#JK(1, 0)
onoremap <silent> ''j :call EasyMotion#JK(0, 0)
nnoremap <silent> ''j :call EasyMotion#JK(0, 0)
vnoremap <silent> ''gE :call EasyMotion#EW(1, 1)
onoremap <silent> ''gE :call EasyMotion#EW(0, 1)
nnoremap <silent> ''gE :call EasyMotion#EW(0, 1)
vnoremap <silent> ''f :call EasyMotion#F(1, 0)
onoremap <silent> ''f :call EasyMotion#F(0, 0)
nnoremap <silent> ''f :call EasyMotion#F(0, 0)
vnoremap <silent> ''e :call EasyMotion#E(1, 0)
onoremap <silent> ''e :call EasyMotion#E(0, 0)
nnoremap <silent> ''e :call EasyMotion#E(0, 0)
vnoremap <silent> ''b :call EasyMotion#WB(1, 1)
onoremap <silent> ''b :call EasyMotion#WB(0, 1)
nnoremap <silent> ''b :call EasyMotion#WB(0, 1)
vnoremap <silent> ''W :call EasyMotion#WBW(1, 0)
onoremap <silent> ''W :call EasyMotion#WBW(0, 0)
nnoremap <silent> ''W :call EasyMotion#WBW(0, 0)
vnoremap <silent> ''T :call EasyMotion#T(1, 1)
onoremap <silent> ''T :call EasyMotion#T(0, 1)
nnoremap <silent> ''T :call EasyMotion#T(0, 1)
vnoremap <silent> ''N :call EasyMotion#Search(1, 1)
onoremap <silent> ''N :call EasyMotion#Search(0, 1)
nnoremap <silent> ''N :call EasyMotion#Search(0, 1)
vnoremap <silent> ''ge :call EasyMotion#E(1, 1)
onoremap <silent> ''ge :call EasyMotion#E(0, 1)
nnoremap <silent> ''ge :call EasyMotion#E(0, 1)
vnoremap <silent> ''F :call EasyMotion#F(1, 1)
onoremap <silent> ''F :call EasyMotion#F(0, 1)
nnoremap <silent> ''F :call EasyMotion#F(0, 1)
vnoremap <silent> ''E :call EasyMotion#EW(1, 0)
onoremap <silent> ''E :call EasyMotion#EW(0, 0)
nnoremap <silent> ''E :call EasyMotion#EW(0, 0)
vnoremap <silent> ''B :call EasyMotion#WBW(1, 1)
onoremap <silent> ''B :call EasyMotion#WBW(0, 1)
nnoremap <silent> ''B :call EasyMotion#WBW(0, 1)
nnoremap <silent> 't :CtrlP
nmap <silent> 'ubs :call BASH_RemoveGuiMenus()
nmap <silent> 'lbs :call BASH_CreateGuiMenus()
nmap 'ihn :IHN
nmap 'is :IHS:A
nmap 'ih :IHS
nnoremap <silent> 'K :call ri#LookupNameUnderCursor()
nnoremap 's :Ag --ignore tags 
nnoremap 'jd :YcmCompleter GoTo
nmap 'b :CtrlPMRUFiles
nmap 'p "+p
vnoremap 'y "+y
noremap ; :
nmap F ''F
nmap K :Man =expand('<cword>')
xmap S <Plug>VSurround
vmap [% [%m'gv``
nnoremap \ea :call setline('.', getline('.') . ';')
nnoremap \ed :call setline('.', getline('.')[:-2])
vmap ]% ]%m'gv``
vmap a% [%v]%
nmap cs <Plug>Csurround
nnoremap co :copen
nmap ds <Plug>Dsurround
nmap f ''f
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
xmap gS <Plug>VgSurround
map mp <Plug>Vm_goto_prev_sign
map mn <Plug>Vm_goto_next_sign
map <silent> mr <Plug>Vm_toggle_sign 
nnoremap mk :make -j4 
nnoremap mc :make clean 
nnoremap mm :make 
nnoremap <silent> tg :!ctags -R
nmap wc :+quit
nnoremap wl l
nnoremap wk k
nnoremap wj j
nnoremap wh h
nmap ySS <Plug>YSsurround
nmap ySs <Plug>YSsurround
nmap yss <Plug>Yssurround
nmap yS <Plug>YSurround
nmap ys <Plug>Ysurround
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(expand((exists("g:netrw_gx")? g:netrw_gx : '<cfile>')),netrw#CheckIfRemote())
nnoremap <silent> <Plug>SurroundRepeat .
snoremap <silent> <Del> c
snoremap <silent> <BS> c
snoremap <silent> <C-Tab> :call UltiSnips#ListSnippets()
xnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("x", "Uncomment")
nnoremap <silent> <Plug>NERDCommenterUncomment :call NERDComment("n", "Uncomment")
xnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("x", "AlignBoth")
nnoremap <silent> <Plug>NERDCommenterAlignBoth :call NERDComment("n", "AlignBoth")
xnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("x", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAlignLeft :call NERDComment("n", "AlignLeft")
nnoremap <silent> <Plug>NERDCommenterAppend :call NERDComment("n", "Append")
xnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("x", "Yank")
nnoremap <silent> <Plug>NERDCommenterYank :call NERDComment("n", "Yank")
xnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("x", "Sexy")
nnoremap <silent> <Plug>NERDCommenterSexy :call NERDComment("n", "Sexy")
xnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("x", "Invert")
nnoremap <silent> <Plug>NERDCommenterInvert :call NERDComment("n", "Invert")
nnoremap <silent> <Plug>NERDCommenterToEOL :call NERDComment("n", "ToEOL")
xnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("x", "Nested")
nnoremap <silent> <Plug>NERDCommenterNested :call NERDComment("n", "Nested")
xnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("x", "Minimal")
nnoremap <silent> <Plug>NERDCommenterMinimal :call NERDComment("n", "Minimal")
xnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("x", "Toggle")
nnoremap <silent> <Plug>NERDCommenterToggle :call NERDComment("n", "Toggle")
xnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("x", "Comment")
nnoremap <silent> <Plug>NERDCommenterComment :call NERDComment("n", "Comment")
nnoremap <silent> <F11> :call conque_term#exec_file()
map <silent> <F6> :TagbarToggle
map <silent> <F5> :NERDTreeToggle
map <silent> <F4> :nohlsearch
imap S <Plug>ISurround
imap s <Plug>Isurround
inoremap <expr> 	 pumvisible() ? "\" : "\	"
imap  <Plug>Isurround
inoremap <silent> '	 =UltiSnips#ExpandSnippetOrJump()
imap 'ihn :IHN
imap 'is :IHS:A
imap 'ih :IHS
cnoremap RC :e $MYVIMRC
inoremap \ea :call setline('.', getline('.') . ';')a
inoremap \ed :call setline('.', getline('.')[:-2])a
cnoremap bb :b#
let &cpo=s:cpo_save
unlet s:cpo_save
set autoread
set autowrite
set background=dark
set backspace=eol,start,indent
set complete=.,w,b,u,t,i,k
set completefunc=youcompleteme#Complete
set completeopt=preview,menuone
set cpoptions=aAceFsB
set fileencodings=utf-8,gb2312,gbk,gb18030
set helplang=en
set hidden
set history=40
set hlsearch
set ignorecase
set incsearch
set laststatus=2
set lazyredraw
set mouse=nv
set omnifunc=youcompleteme#OmniComplete
set path=./,./include/,../,/usr/include/,/usr/include/i386-linux-gnu/,/usr/local/include/,/
set ruler
set runtimepath=~/.vim,~/.vim/bundle/Tabular,~/.vim/bundle/YouCompleteMe,~/.vim/bundle/a,~/.vim/bundle/ag-master,~/.vim/bundle/airline,~/.vim/bundle/bash-support,~/.vim/bundle/c-support,~/.vim/bundle/coffee,~/.vim/bundle/conque,~/.vim/bundle/css-completion,~/.vim/bundle/ctrlp.vim-master,~/.vim/bundle/ctrlsf,~/.vim/bundle/delimitMate,~/.vim/bundle/easymotion,~/.vim/bundle/eclim,~/.vim/bundle/elixir,~/.vim/bundle/erl_def,~/.vim/bundle/erlang-tags,~/.vim/bundle/fswitch,~/.vim/bundle/golang,~/.vim/bundle/haml,~/.vim/bundle/html5.vim-master,~/.vim/bundle/javascript,~/.vim/bundle/lua-inspect,~/.vim/bundle/markdown,~/.vim/bundle/matchit,~/.vim/bundle/misc,~/.vim/bundle/nerdcommenter-master,~/.vim/bundle/nerdtree,~/.vim/bundle/perl-support,~/.vim/bundle/protobuf,~/.vim/bundle/protodef,~/.vim/bundle/ruby,~/.vim/bundle/scala,~/.vim/bundle/slim,~/.vim/bundle/sproto,~/.vim/bundle/tagbar,~/.vim/bundle/tern_for_vim-master,~/.vim/bundle/ultisnips,~/.vim/bundle/utl.vim-master,~/.vim/bundle/vim-colors-solarized,~/.vim/bundle/vim-csharp,~/.vim/bundle/vim-ember,~/.vim/bundle/vim-ember-hbs,~/.vim/bundle/vim-fugitive,~/.vim/bundle/vim-glsl,~/.vim/bundle/vim-gradle,~/.vim/bundle/vim-lua-ftplugin-master,~/.vim/bundle/vim-misc-master,~/.vim/bundle/vim-multiple-cursors,~/.vim/bundle/vim-notes-master,~/.vim/bundle/vim-octave,~/.vim/bundle/vim-rails,~/.vim/bundle/vim-snippets,~/.vim/bundle/vim-solidity,~/.vim/bundle/vim-surround,~/.vim/bundle/vim-vue,~/.vim/bundle/vimerl-master,~/.vim/bundle/visualmark-master,~/.vim/bundle/xml.vim-master,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,~/.vim/bundle/Tabular/after,~/.vim/bundle/coffee/after,~/.vim/bundle/ctrlsf/after,~/.vim/bundle/scala/after,~/.vim/bundle/tern_for_vim-master/after,~/.vim/bundle/ultisnips/after,~/.vim/bundle/vim-glsl/after,~/.vim/after,~/.vim/eclim,~/.vim/eclim/after
set shortmess=filnxtToOc
set smartcase
set splitbelow
set splitright
set termencoding=utf-8
set updatetime=2000
set viminfo='20,\"100,:20,n~/.viminfo
set whichwrap=b,s,<,>
set wildignore=*.o,*.obj,*.beam,*/.git/*,*/.hg/*,*/.svn/*
set wildmenu
set wildmode=longest:full,full
set nowrapscan
" vim: set ft=vim :
