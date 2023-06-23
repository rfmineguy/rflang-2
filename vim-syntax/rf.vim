" Vim syntax file
" Language: rf
" Maintainer: Riley Fischer

" Finish early if a syntax was already selected
if exists("b:current_syntax")
  finish
endif

" Keywords
syn keyword KeywordDataTypes int char short double float bool 
syn keyword KeywordVoid void
syn keyword LanguageStatements for if while return asm fn 
syn keyword Define def
syn keyword Module use
syn keyword AsmTypes x86_64-linux x86_32-linux arm64

"Block
"syn region codeBlock start=+{+ end=+}+ keepend
syn region StringLiteral start='"' end='"' contained

syn case ignore
syn match bNumber "\<[0-9]\+>"
syn case match

" Highlighting
hi def link KeywordDataTypes Type
hi def link KeywordVoid Type
hi def link KeywordFlow Repeat
hi def link LanguageStatements Statement
hi def link Define Preproc
hi def link Module PreProc
hi def link bNumber Number
hi def link AsmTypes Type
hi def link StringLiteral Constant

let b:current_syntax = "rf"
