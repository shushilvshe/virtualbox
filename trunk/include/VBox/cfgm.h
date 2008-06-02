/** @file
 * CFGM - Configuration Manager
 */

/*
 * Copyright (C) 2006-2007 Sun Microsystems, Inc.
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 *
 * The contents of this file may alternatively be used under the terms
 * of the Common Development and Distribution License Version 1.0
 * (CDDL) only, as it comes in the "COPYING.CDDL" file of the
 * VirtualBox OSE distribution, in which case the provisions of the
 * CDDL are applicable instead of those of the GPL.
 *
 * You may elect to license modified versions of this file under the
 * terms and conditions of either the GPL or the CDDL or both.
 *
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 USA or visit http://www.sun.com if you need
 * additional information or have any questions.
 */

#ifndef ___VBox_cfgm_h
#define ___VBox_cfgm_h

#include <VBox/cdefs.h>
#include <VBox/types.h>
#include <iprt/stdarg.h>

/** @defgroup   grp_cfgm        The Configuration Manager API
 * @{
 */

/** Configuration manager tree node - A key. */
typedef struct CFGMNODE *PCFGMNODE;

/** Configuration manager tree leaf - A value. */
typedef struct CFGMLEAF *PCFGMLEAF;

/**
 * Configuration manager value type.
 */
typedef enum CFGMVALUETYPE
{
    /** Integer value. */
    CFGMVALUETYPE_INTEGER = 1,
    /** String value. */
    CFGMVALUETYPE_STRING,
    /** Bytestring value. */
    CFGMVALUETYPE_BYTES
} CFGMVALUETYPE;
/** Pointer to configuration manager property type. */
typedef CFGMVALUETYPE *PCFGMVALUETYPE;



__BEGIN_DECLS

#ifdef IN_RING3
/** @defgroup   grp_cfgm_r3     The CFGM Host Context Ring-3 API
 * @ingroup grp_cfgm
 * @{
 */

typedef enum CFGMCONFIGTYPE
{
    /** pvConfig points to nothing, use defaults. */
    CFGMCONFIGTYPE_NONE = 0,
    /** pvConfig points to a IMachine interface. */
    CFGMCONFIGTYPE_IMACHINE
} CFGMCONFIGTYPE;


/**
 * CFGM init callback for constructing the configuration tree.
 *
 * This is called from the emulation thread, and the one interfacing the VM
 * can make any necessary per-thread initializations at this point.
 *
 * @returns VBox status code.
 * @param   pVM             VM handle.
 * @param   pvUser          The argument supplied to VMR3Create().
 */
typedef DECLCALLBACK(int) FNCFGMCONSTRUCTOR(PVM pVM, void *pvUser);
/** Pointer to a FNCFGMCONSTRUCTOR(). */
typedef FNCFGMCONSTRUCTOR *PFNCFGMCONSTRUCTOR;

CFGMR3DECL(int)         CFGMR3Init(PVM pVM, PFNCFGMCONSTRUCTOR pfnCFGMConstructor, void *pvUser);
CFGMR3DECL(int)         CFGMR3Term(PVM pVM);


CFGMR3DECL(PCFGMNODE)   CFGMR3CreateTree(PVM pVM);
CFGMR3DECL(void)        CFGMR3Dump(PCFGMNODE pRoot);
CFGMR3DECL(int)         CFGMR3InsertSubTree(PCFGMNODE pNode, const char *pszName, PCFGMNODE pSubTree, PCFGMNODE *ppChild);
CFGMR3DECL(int)         CFGMR3InsertNode(PCFGMNODE pNode, const char *pszName, PCFGMNODE *ppChild);
CFGMR3DECL(int)         CFGMR3InsertNodeF(PCFGMNODE pNode, PCFGMNODE *ppChild, const char *pszNameFormat, ...);
CFGMR3DECL(int)         CFGMR3InsertNodeFV(PCFGMNODE pNode, PCFGMNODE *ppChild, const char *pszNameFormat, va_list Args);
CFGMR3DECL(void)        CFGMR3SetRestrictedRoot(PCFGMNODE pNode);
CFGMR3DECL(void)        CFGMR3RemoveNode(PCFGMNODE pNode);
CFGMR3DECL(int)         CFGMR3InsertInteger(PCFGMNODE pNode, const char *pszName, uint64_t u64Integer);
CFGMR3DECL(int)         CFGMR3InsertString(PCFGMNODE pNode, const char *pszName, const char *pszString);
CFGMR3DECL(int)         CFGMR3InsertBytes(PCFGMNODE pNode, const char *pszName, const void *pvBytes, size_t cbBytes);
CFGMR3DECL(int)         CFGMR3RemoveValue(PCFGMNODE pNode, const char *pszName);

CFGMR3DECL(int)         CFGMR3QueryType(        PCFGMNODE pNode, const char *pszName, PCFGMVALUETYPE penmType);
CFGMR3DECL(int)         CFGMR3QuerySize(        PCFGMNODE pNode, const char *pszName, size_t *pcb);
CFGMR3DECL(int)         CFGMR3QueryInteger(     PCFGMNODE pNode, const char *pszName, uint64_t *pu64);
CFGMR3DECL(int)         CFGMR3QueryIntegerDef(  PCFGMNODE pNode, const char *pszName, uint64_t *pu64, uint64_t u64Def);
CFGMR3DECL(int)         CFGMR3QueryString(      PCFGMNODE pNode, const char *pszName, char *pszString, size_t cchString);
CFGMR3DECL(int)         CFGMR3QueryStringDef(   PCFGMNODE pNode, const char *pszName, char *pszString, size_t cchString, const char *pszDef);
CFGMR3DECL(int)         CFGMR3QueryBytes(       PCFGMNODE pNode, const char *pszName, void *pvData, size_t cbData);


/** @name Helpers
 * @{
 */
CFGMR3DECL(int)         CFGMR3QueryU64(         PCFGMNODE pNode, const char *pszName, uint64_t *pu64);
CFGMR3DECL(int)         CFGMR3QueryU64Def(      PCFGMNODE pNode, const char *pszName, uint64_t *pu64, uint64_t u64Def);
CFGMR3DECL(int)         CFGMR3QueryS64(         PCFGMNODE pNode, const char *pszName, int64_t *pi64);
CFGMR3DECL(int)         CFGMR3QueryS64Def(      PCFGMNODE pNode, const char *pszName, int64_t *pi64, int64_t i64Def);
CFGMR3DECL(int)         CFGMR3QueryU32(         PCFGMNODE pNode, const char *pszName, uint32_t *pu32);
CFGMR3DECL(int)         CFGMR3QueryU32Def(      PCFGMNODE pNode, const char *pszName, uint32_t *pu32, uint32_t u32Def);
CFGMR3DECL(int)         CFGMR3QueryS32(         PCFGMNODE pNode, const char *pszName, int32_t *pi32);
CFGMR3DECL(int)         CFGMR3QueryS32Def(      PCFGMNODE pNode, const char *pszName, int32_t *pi32, int32_t i32Def);
CFGMR3DECL(int)         CFGMR3QueryU16(         PCFGMNODE pNode, const char *pszName, uint16_t *pu16);
CFGMR3DECL(int)         CFGMR3QueryU16Def(      PCFGMNODE pNode, const char *pszName, uint16_t *pu16, uint16_t u16Def);
CFGMR3DECL(int)         CFGMR3QueryS16(         PCFGMNODE pNode, const char *pszName, int16_t *pi16);
CFGMR3DECL(int)         CFGMR3QueryS16Def(      PCFGMNODE pNode, const char *pszName, int16_t *pi16, int16_t i16Def);
CFGMR3DECL(int)         CFGMR3QueryU8(          PCFGMNODE pNode, const char *pszName, uint8_t *pu8);
CFGMR3DECL(int)         CFGMR3QueryU8Def(       PCFGMNODE pNode, const char *pszName, uint8_t *pu8, uint8_t u8Def);
CFGMR3DECL(int)         CFGMR3QueryS8(          PCFGMNODE pNode, const char *pszName, int8_t *pi8);
CFGMR3DECL(int)         CFGMR3QueryS8Def(       PCFGMNODE pNode, const char *pszName, int8_t *pi8, int8_t i8Def);
CFGMR3DECL(int)         CFGMR3QueryBool(        PCFGMNODE pNode, const char *pszName, bool *pf);
CFGMR3DECL(int)         CFGMR3QueryBoolDef(     PCFGMNODE pNode, const char *pszName, bool *pf, bool fDef);
CFGMR3DECL(int)         CFGMR3QueryPort(        PCFGMNODE pNode, const char *pszName, PRTIOPORT pPort);
CFGMR3DECL(int)         CFGMR3QueryPortDef(     PCFGMNODE pNode, const char *pszName, PRTIOPORT pPort, RTIOPORT PortDef);
CFGMR3DECL(int)         CFGMR3QueryPtr(         PCFGMNODE pNode, const char *pszName, void **ppv);
CFGMR3DECL(int)         CFGMR3QueryPtrDef(      PCFGMNODE pNode, const char *pszName, void **ppv, void *pvDef);
CFGMR3DECL(int)         CFGMR3QueryGCPtr(       PCFGMNODE pNode, const char *pszName, PRTGCPTR pGCPtr);
CFGMR3DECL(int)         CFGMR3QueryGCPtrDef(    PCFGMNODE pNode, const char *pszName, PRTGCPTR pGCPtr, RTGCPTR GCPtrDef);
CFGMR3DECL(int)         CFGMR3QueryGCPtrU(      PCFGMNODE pNode, const char *pszName, PRTGCUINTPTR pGCPtr);
CFGMR3DECL(int)         CFGMR3QueryGCPtrUDef(   PCFGMNODE pNode, const char *pszName, PRTGCUINTPTR pGCPtr, RTGCUINTPTR GCPtrDef);
CFGMR3DECL(int)         CFGMR3QueryGCPtrS(      PCFGMNODE pNode, const char *pszName, PRTGCINTPTR pGCPtr);
CFGMR3DECL(int)         CFGMR3QueryGCPtrSDef(   PCFGMNODE pNode, const char *pszName, PRTGCINTPTR pGCPtr, RTGCINTPTR GCPtrDef);
CFGMR3DECL(int)         CFGMR3QueryStringAlloc( PCFGMNODE pNode, const char *pszName, char **ppszString);
CFGMR3DECL(int)         CFGMR3QueryStringAllocDef(PCFGMNODE pNode, const char *pszName, char **ppszString, const char *pszDef);

/** @} */

/** @name Tree Navigation and Enumeration.
 * @{
 */
CFGMR3DECL(PCFGMNODE)   CFGMR3GetRoot(PVM pVM);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetParent(PCFGMNODE pNode);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetParentEx(PVM pVM, PCFGMNODE pNode);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetChild(PCFGMNODE pNode, const char *pszPath);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetChildF(PCFGMNODE pNode, const char *pszPathFormat, ...);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetChildFV(PCFGMNODE pNode, const char *pszPathFormat, va_list Args);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetFirstChild(PCFGMNODE pNode);
CFGMR3DECL(PCFGMNODE)   CFGMR3GetNextChild(PCFGMNODE pCur);
CFGMR3DECL(int)         CFGMR3GetName(PCFGMNODE pCur, char *pszName, size_t cchName);
CFGMR3DECL(int)         CFGMR3GetNameLen(PCFGMNODE pCur);
CFGMR3DECL(bool)        CFGMR3AreChildrenValid(PCFGMNODE pNode, const char *pszzValid);
CFGMR3DECL(PCFGMLEAF)   CFGMR3GetFirstValue(PCFGMNODE pCur);
CFGMR3DECL(PCFGMLEAF)   CFGMR3GetNextValue(PCFGMLEAF pCur);
CFGMR3DECL(int)         CFGMR3GetValueName(PCFGMLEAF pCur, char *pszName, size_t cchName);
CFGMR3DECL(int)         CFGMR3GetValueNameLen(PCFGMLEAF pCur);
CFGMR3DECL(CFGMVALUETYPE) CFGMR3GetValueType(PCFGMLEAF pCur);
CFGMR3DECL(bool)        CFGMR3AreValuesValid(PCFGMNODE pNode, const char *pszzValid);
/** @} */


/** @} */
#endif  /* IN_RING3 */


__END_DECLS

/** @} */

#endif

