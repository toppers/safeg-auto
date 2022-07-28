##コンフィグファイルパーサークラス

from typing import *

from parse_base import *


from def_cfg_kw import *
from yaml_node import *
from parse_util import *

from def_cfg import *

from cfg import *

from cfg_vcom import *
from cfg_vcom_svar import *
from cfg_vcom_msgq import *
from cfg_vcom_sbuf import *

from error import *


class Parser_VCOM_t(ParserBase_t):
	#ctor
	def __init__(self, cfg: Cfg_t) -> None:
		super().__init__(cfg)


	#VCOMノード
	def Parse(self, node: Node_t):
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_STATEVAR,		True),
				NodeDef_t(CFG_KW_MSG_QUEUE,		True),
				NodeDef_t(CFG_KW_SHARED_BUF,	True)]
		#キーワードチェック
		node.chkKeyword(keys)

		#VCOMオブジェクト
		vcom = self.Cfg.VCOM

		#StateVariable
		nodeStateVar = node.get(CFG_KW_STATEVAR, True)
		if nodeStateVar:
			self.parseStateVar(nodeStateVar, vcom.SVars)

		#MessageQueue
		nodeMsgQ = node.get(CFG_KW_MSG_QUEUE, True)
		if nodeMsgQ:
			self.parseMsgQueue(nodeMsgQ, vcom.MsgQs)

		#SharedBuffer
		nodeSharedBuf = node.get(CFG_KW_SHARED_BUF, True)
		if nodeSharedBuf:
			self.parseSharedBuf(nodeSharedBuf, vcom.SBufs)

		return


	#StateVariableノード
	def parseStateVar(self, nodes: Node_t, stateVars: Type[SVar_t]):
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_NAME,			True),
				NodeDef_t(CFG_KW_SIZE,			True),
				NodeDef_t(CFG_KW_WRITABLE_VM,	True),
				NodeDef_t(CFG_KW_INIT_STATE,	True)]

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#オブジェクト生成
			obj = SVar_t()

			#Name
			#err:識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)
				#self.existsName(name, node.LastName)
				obj.Name = name

			#Size
			size = node.getInt(CFG_KW_SIZE)
			obj.Size = size

			#WritebleVM
			wrVM = node.getStr(CFG_KW_WRITABLE_VM)
			if wrVM:
				#VMが存在しないか
				if not wrVM in self.Cfg.VMs:
					#err:VMが存在しない
					AppErrorLastNode(node, wrVM, 'is not defined')
				else:
					obj.wrVM = self.Cfg.VMs[wrVM]

			#InitState
			init = node.getBool(CFG_KW_INIT_STATE)
			obj.Init = init

			#クラスにインスタンス追加
			if name:
				stateVars.AddInstance(obj)

		#1個でも存在すればHVCを追加
		if stateVars:
			self.Cfg.HV.AllHVCs.extend(SVar_t.HVCs)



	#MessageQueueノード
	def parseMsgQueue(self, nodes: Node_t, msgQs: Type[MsgQ_t]):
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_NAME,				True),
				NodeDef_t(CFG_KW_MAX_SIZE,			True),
				NodeDef_t(CFG_KW_MSG_QUEUE_SIZE,	True),
				NodeDef_t(CFG_KW_WRITABLE_VM,		True),
				NodeDef_t(CFG_KW_READABLE_VM,		True),
				NodeDef_t(CFG_KW_INIT_STATE,		True)]

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#オブジェクト生成
			obj = MsgQ_t()

			#Name
			#err:識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			#同名のチェック
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)
				#self.existsName(name, node.LastName)
				obj.Name = name

			#MsgQSize
			size = node.getInt(CFG_KW_MSG_QUEUE_SIZE)
			obj.Size = size

			#MaxSize
			maxSize = node.getInt(CFG_KW_MAX_SIZE)
			obj.MaxSize = maxSize

			#サイズ関係チェック
			if maxSize > size:
				#err:MaxSize <= MsgSize
				AppErrorLastNode(node, maxSize, f'must be <= {CFG_KW_MSG_QUEUE_SIZE}({size})')

			#WritableVM
			wrVM = node.getStr(CFG_KW_WRITABLE_VM)
			if wrVM:
				#VMが存在しないか
				if not wrVM in self.Cfg.VMs:
					#err:VMが存在しない
					AppErrorLastNode(node, wrVM, 'is not defined')
				else:
					obj.wrVM = self.Cfg.VMs[wrVM]

			#ReadableVM
			rdVM = node.getStr(CFG_KW_READABLE_VM)
			if rdVM:
				#VMが存在しないか
				if not rdVM in self.Cfg.VMs:
					#err:VMが存在しない
					AppErrorLastNode(node, rdVM, 'is not defined')
				else:
					obj.rdVM = self.Cfg.VMs[rdVM]

			#InitState
			init = node.getBool(CFG_KW_INIT_STATE)
			obj.Init = init


			#クラスにインスタンス追加
			if name:
				msgQs.AddInstance(obj)


		#1個でも存在すればHVCを追加
		if msgQs:
			self.Cfg.HV.AllHVCs.extend(MsgQ_t.HVCs)


	#SharedBufノード
	def parseSharedBuf(self, nodes: Node_t, sBufs: Type[SBuf_t]):
		#有効キーワード
		keys = [NodeDef_t(CFG_KW_NAME,			True),
				NodeDef_t(CFG_KW_SIZE,			True),
				NodeDef_t(CFG_KW_ACCESIBLE_VM,	True)]

		for node in nodes.array():
			#キーワードチェック
			node.chkKeyword(keys)

			#オブジェクト生成
			obj = SBuf_t()

			#Name
			#err:識別子として無効
			name = node.getIdentifier(CFG_KW_NAME)
			#同名のチェック
			if name:
				#err:同名のオブジェクトが存在するか
				self.Cfg.existName(name, node.LastName)
				#self.existsName(name, node.LastName)
				obj.Name	= name

			#Size
			size = node.getInt(CFG_KW_SIZE)
			obj.Size	= size

			#AccesibleVM
			nodeVMs = node.get(CFG_KW_ACCESIBLE_VM)
			if nodeVMs:
				for nodeVM in nodeVMs.array():
					vm = nodeVM.asStr()
					#VMが存在しないか
					if not vm in self.Cfg.VMs:
						#err:VMが存在しない
						AppErrorLastNode(nodeVM, vm, 'is not defined')
					elif next((v for v in obj.AccesVMs if v ==vm), None):
						#err:リストに既に存在する
						AppErrorNode(nodeVM, 'is already registered')
					else:
						obj.AccesVMs.append(self.Cfg.VMs[vm])

			#クラスにインスタンス追加
			if name:
				sBufs.AddInstance(obj)


		#1個でも存在すればHVCを追加
		if sBufs:
			self.Cfg.HV.AllHVCs.extend(SBuf_t.HVCs)

