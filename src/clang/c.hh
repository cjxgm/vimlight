#pragma once
// a direct wrapper to libclang c interface, thus the name.
//
// this wrapper's goal is to make the interface conform to
// the standard c++ coding style and naming scheme (aka STL
// naming scheme).
//
// only functions/types/enums that is used by the project
// if forwarded.
//
#include <clang-c/Index.h>
#include <utility>

#define TYPE(NAME)			using type = ::CX ## NAME
#define FUNC(NAME, CNAME)	constexpr auto NAME = ::clang_ ## CNAME
#define ENUM(NAME, CNAME)	constexpr auto NAME = ::CX ## CNAME

namespace clang
{
	namespace c
	{
		namespace index
		{
			TYPE(Index);
			FUNC(create, createIndex);
			FUNC(dispose, disposeIndex);
		}

		namespace translation_unit
		{
			TYPE(TranslationUnit);
			FUNC(parse, parseTranslationUnit);
			FUNC(dispose, disposeTranslationUnit);
			FUNC(reparse, reparseTranslationUnit);
			FUNC(get_cursor, getTranslationUnitCursor);
			namespace flag
			{
				TYPE(TranslationUnit_Flags);
				ENUM(none, TranslationUnit_None);
				ENUM(incomplete, TranslationUnit_Incomplete);
			}
		}

		namespace cursor
		{
			TYPE(Cursor);
			FUNC(get_extent, getCursorExtent);
			FUNC(get_location, getCursorLocation);
			FUNC(get_kind, getCursorKind);
			FUNC(get_spelling, getCursorSpelling);
			FUNC(get_type, getCursorType);
			FUNC(get_referenced, getCursorReferenced);
			namespace childs
			{
				namespace visit_result
				{
					TYPE(ChildVisitResult);
					ENUM(into, ChildVisit_Recurse);
					ENUM(next, ChildVisit_Continue);
					ENUM(stop, ChildVisit_Break);
				}
				FUNC(visit, visitChildren);
			}
			namespace kind
			{
				TYPE(CursorKind);
				FUNC(get_spelling, getCursorKindSpelling);
				FUNC(is_declaration, isDeclaration);
				FUNC(is_reference, isReference);
			}
		}

		namespace range
		{
			TYPE(SourceRange);
			FUNC(get_start, getRangeStart);
			FUNC(get_end, getRangeEnd);
		}

		namespace location
		{
			TYPE(SourceLocation);
			FUNC(get_spelling, getSpellingLocation);
			FUNC(is_from_main_file, Location_isFromMainFile);
			FUNC(get_file, getFileLocation);
		}

		namespace string
		{
			TYPE(String);
			FUNC(dispose, disposeString);
			FUNC(cstr, getCString);
		}

		namespace unsaved_file
		{
			TYPE(UnsavedFile);
		}

		namespace file
		{
			TYPE(File);
			FUNC(get_name, getFileName);
		}

		namespace options
		{
			FUNC(default_reparse, defaultReparseOptions);
			FUNC(default_diagnostic_display, defaultDiagnosticDisplayOptions);
		}

		namespace diagnostic
		{
			namespace set
			{
				TYPE(DiagnosticSet);
				FUNC(dispose, disposeDiagnosticSet);
				FUNC(from_translation_unit, getDiagnosticSetFromTU);
				FUNC(get, getDiagnosticInSet);
				FUNC(get_count, getNumDiagnosticsInSet);
			}
			TYPE(Diagnostic);
			FUNC(dispose, disposeDiagnostic);
			FUNC(get_childs, getChildDiagnostics);
			FUNC(get_location, getDiagnosticLocation);
			FUNC(get_range, getDiagnosticRange);
			FUNC(get_range_count, getDiagnosticNumRanges);
			FUNC(format, formatDiagnostic);
		}
	}
}

#undef ENUM
#undef FUNC
#undef TYPE

