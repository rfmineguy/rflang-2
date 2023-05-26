" Vim syntax file
" Language: rf
" Maintainer: Riley Fischer

" Finish early if a syntax was already selected
if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword KeywordTypes int char short double float bool 
syn keyword KeywordFlow for if while return
syn keyword LanguageConstructs asm fn 
syn keyword Preproc use
syn keyword AsmTypes x86_64-linux x86_32-linux arm64

"Block
"syn region codeBlock start=+{+ end=+}+ keepend
syn region StringLiteral start='"' end='"' contained

syn case ignore
syn match bNumber "\<[0-9]\+>"
syn case match

" Highlighting
hi def link KeywordTypes Type
hi def link KeywordFlow Repeat
hi def link LanguageConstructs Statement
hi def link bNumber Number
hi def link AsmTypes Type
hi def link Preproc PreProc
hi def link StringLiteral Constant

let b:current_syntax = "rf"
