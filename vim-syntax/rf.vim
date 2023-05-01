" Vim syntax file
" Language: rflang
" Maintainer: Riley Fischer
" Latest Revision: Apr 30, 2023

" Finish early if a syntax was already selected
if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword basicLanguageKeywordTypes int char short double float bool 
syn keyword basicLanguageKeywordFlow for if while
syn keyword basicLanguageConstructs asm
