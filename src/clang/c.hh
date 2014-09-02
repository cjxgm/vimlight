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




namespace clang
{
	namespace c
	{
#define FORWARD_TYPE(NAME) \
		using type = ::CX ## NAME

#define FORWARD_FUNC(NAME, CNAME) \
		constexpr auto NAME = ::clang_ ## CNAME

#define FORWARD_ENUM(NAME, CNAME) \
		constexpr auto NAME = ::CX ## CNAME




		namespace index
		{
			FORWARD_TYPE(Index);
			FORWARD_FUNC(create, createIndex);
			FORWARD_FUNC(dispose, disposeIndex);
		};

		namespace translation_unit
		{
			FORWARD_TYPE(TranslationUnit);
			FORWARD_FUNC(parse, parseTranslationUnit);
			FORWARD_FUNC(dispose, disposeTranslationUnit);
			FORWARD_FUNC(reparse, reparseTranslationUnit);
			FORWARD_FUNC(get_cursor, getTranslationUnitCursor);
			namespace flag
			{
				FORWARD_TYPE(TranslationUnit_Flags);
				FORWARD_ENUM(none, TranslationUnit_None);
				FORWARD_ENUM(incomplete, TranslationUnit_Incomplete);
			};
		};

		namespace cursor
		{
			FORWARD_TYPE(Cursor);
			FORWARD_FUNC(get_extent, getCursorExtent);
			FORWARD_FUNC(get_kind, getCursorKind);
			FORWARD_FUNC(get_spelling, getCursorSpelling);
			namespace childs
			{
				namespace visit_result
				{
					FORWARD_TYPE(ChildVisitResult);
					FORWARD_ENUM(into, ChildVisit_Recurse);
					FORWARD_ENUM(next, ChildVisit_Continue);
					FORWARD_ENUM(stop, ChildVisit_Break);
				};
				FORWARD_FUNC(visit, visitChildren);
			};
			namespace kind
			{
				FORWARD_TYPE(CursorKind);
				FORWARD_FUNC(get_spelling, getCursorKindSpelling);
				FORWARD_FUNC(is_declaration, isDeclaration);
				FORWARD_FUNC(is_reference, isReference);
			};
		};

		namespace range
		{
			FORWARD_TYPE(SourceRange);
			FORWARD_FUNC(get_start, getRangeStart);
			FORWARD_FUNC(get_end, getRangeEnd);
		};

		namespace location
		{
			FORWARD_TYPE(SourceLocation);
			FORWARD_FUNC(get_spelling, getSpellingLocation);
			FORWARD_FUNC(is_from_main_file, Location_isFromMainFile);
		};

		namespace string
		{
			FORWARD_TYPE(String);
			FORWARD_FUNC(dispose, disposeString);
			FORWARD_FUNC(cstr, getCString);
		};

		namespace unsaved_file
		{
			FORWARD_TYPE(UnsavedFile);
		};

		namespace options
		{
			FORWARD_FUNC(default_reparse, defaultReparseOptions);
			FORWARD_FUNC(default_diagnostic_display, defaultDiagnosticDisplayOptions);
		};

		namespace diagnostic
		{
			FORWARD_TYPE(Diagnostic);
			FORWARD_FUNC(dispose, disposeDiagnostic);
			FORWARD_FUNC(get, getDiagnostic);
			FORWARD_FUNC(get_count, getNumDiagnostics);
			FORWARD_FUNC(get_location, getDiagnosticLocation);
			FORWARD_FUNC(format, formatDiagnostic);
		};




#undef FORWARD_ENUM
#undef FORWARD_FUNC
#undef FORWARD_TYPE
	};
};

