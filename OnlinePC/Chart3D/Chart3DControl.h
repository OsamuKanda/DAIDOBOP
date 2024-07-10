#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace ChartFX::WinForms;

namespace Chart3D {

	/// <summary>
	/// Chart3Dコントロールの概要
	/// </summary>
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	public ref class Chart3DControl : public System::Windows::Forms::UserControl
	{
	public:
		Chart3DControl(void)
		{
			InitializeComponent();
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
			myData1 = gcnew array<int,2>(12,100);
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData1[i,j] = 0;
			myData2 = gcnew array<int,2>(12,100);
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData2[i,j] = 0;
			myData3 = gcnew array<int,2>(12,100);
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData3[i,j] = 0;
			myDataEx = gcnew array<int,1>(4);
			for(int i = 0; i < 4; i++ )
				myDataEx[i] = 0;
			myLabel = gcnew array<int,2>(3,12);
			myLabel[0,0] = 26;
			myLabel[0,1] = 24;
			myLabel[0,2] = 21;
			myLabel[0,3] = 19;
			myLabel[0,4] = 17;
			myLabel[0,5] = 15;
			myLabel[0,6] = 12;
			myLabel[0,7] = 10;
			myLabel[0,8] = 7;
			myLabel[0,9] = 5;
			myLabel[0,10] = 3;
			myLabel[0,11] = 1;
			myLabel[1,0] = 27;
			myLabel[1,1] = 25;
			myLabel[1,2] = 22;
			myLabel[1,3] = 20;
			myLabel[1,4] = 18;
			myLabel[1,5] = 16;
			myLabel[1,6] = 13;
			myLabel[1,7] = 11;
			myLabel[1,8] = 8;
			myLabel[1,9] = 6;
			myLabel[1,10] = 4;
			myLabel[1,11] = 2;
			myLabel[2,0] = 28;
			myLabel[2,1] = 0;
			myLabel[2,2] = 23;
			myLabel[2,3] = 0;
			myLabel[2,4] = 0;
			myLabel[2,5] = 0;
			myLabel[2,6] = 14;
			myLabel[2,7] = 0;
			myLabel[2,8] = 9;
			myLabel[2,9] = 0;
			myLabel[2,10] = 0;
			myLabel[2,11] = 0;
			myTcName = gcnew array<String^,1>(28);
			nDataCnt = 0;
			isSurface = true;
			isDraw = true;
			nLineNo = 0;
			reqChgTcName = false;
		}

		System::Void setData1(int *pData)
		{
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData1[i,j] = pData[j*12+i];
		}
		System::Void setData2(int *pData)
		{
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData2[i,j] = pData[j*12+i];
		}
		System::Void setData3(int *pData)
		{
			for(int i = 0; i < 12; i++ )
				for(int j = 0; j < 100; j++ )
					myData3[i,j] = pData[j*12+i];
		}

		System::Void setDataEx(int *pData)
		{
			for(int i = 0; i < 4; i++ )
				myDataEx[i] = pData[i];
		}

		System::Void chgTcName()
		{
			reqChgTcName = true;
		}

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~Chart3DControl()
		{
			if (components)
			{
				delete components;
			}
		}

	private: ChartFX::WinForms::Chart^  chart1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;



	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;


	protected: 
		array<int,2>	^myData1;
		array<int,2>	^myData2;
		array<int,2>	^myData3;
		array<int,2>	^myLabel;
		array<String^,1>	^myTcName;
		array<int,1>	^myDataEx;
		int				nDataCnt;
		bool			isSurface;
		bool			isDraw;
		int				nLineNo;
		bool			reqChgTcName;
		String^			szAppPath;
		String^			szTcNameFile;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button8;
	private: System::Windows::Forms::Button^  button9;

private: System::Windows::Forms::Panel^  panel1;
private: System::Windows::Forms::Panel^  panel2;
private: System::Windows::Forms::Panel^  panel3;
private: System::Windows::Forms::Panel^  panel4;
private: System::Windows::Forms::Panel^  panel5;
private: System::Windows::Forms::Panel^  panel6;
private: System::Windows::Forms::Panel^  panel7;
private: System::Windows::Forms::Panel^  panel8;
private: System::Windows::Forms::Panel^  panel9;
private: System::Windows::Forms::Panel^  panel10;
private: System::Windows::Forms::Panel^  panel11;
private: System::Windows::Forms::Panel^  panel12;
private: System::Windows::Forms::Panel^  panel13;
private: System::Windows::Forms::Panel^  panel14;
private: System::Windows::Forms::Panel^  panel15;
private: System::Windows::Forms::Label^  label1;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::Label^  label18;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label21;
private: System::Windows::Forms::Label^  label22;
private: System::Windows::Forms::Label^  label23;
private: System::Windows::Forms::Label^  label24;
private: System::Windows::Forms::Label^  label25;
private: System::Windows::Forms::Label^  label26;
private: System::Windows::Forms::Label^  label27;
private: System::Windows::Forms::Label^  label28;
private: System::Windows::Forms::Label^  label29;
private: System::Windows::Forms::Label^  label30;
private: System::Windows::Forms::Label^  label31;
private: System::Windows::Forms::Label^  label32;
private: System::Windows::Forms::Label^  label33;
private: System::Windows::Forms::Label^  label34;
private: System::Windows::Forms::Label^  label35;
private: System::Windows::Forms::Label^  label36;
private: System::Windows::Forms::Label^  label37;
private: System::Windows::Forms::Label^  label38;
private: System::Windows::Forms::Label^  label39;
private: System::Windows::Forms::Label^  label40;
private: System::Windows::Forms::Label^  label41;
private: System::Windows::Forms::Label^  label42;
private: System::Windows::Forms::Label^  label43;
private: System::Windows::Forms::Label^  label44;
private: System::Windows::Forms::Label^  label45;
private: System::Windows::Forms::Label^  label46;
private: System::Windows::Forms::Label^  label47;
private: System::Windows::Forms::Label^  label48;
private: System::Windows::Forms::Label^  label49;
private: System::Windows::Forms::Label^  label50;
private: System::Windows::Forms::Label^  label51;
private: System::Windows::Forms::Label^  label52;
	protected: 
	public:
		int				testtest;


	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->chart1 = (gcnew ChartFX::WinForms::Chart());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->panel6 = (gcnew System::Windows::Forms::Panel());
			this->panel7 = (gcnew System::Windows::Forms::Panel());
			this->panel8 = (gcnew System::Windows::Forms::Panel());
			this->panel9 = (gcnew System::Windows::Forms::Panel());
			this->panel10 = (gcnew System::Windows::Forms::Panel());
			this->panel11 = (gcnew System::Windows::Forms::Panel());
			this->panel12 = (gcnew System::Windows::Forms::Panel());
			this->panel13 = (gcnew System::Windows::Forms::Panel());
			this->panel14 = (gcnew System::Windows::Forms::Panel());
			this->panel15 = (gcnew System::Windows::Forms::Panel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->label27 = (gcnew System::Windows::Forms::Label());
			this->label28 = (gcnew System::Windows::Forms::Label());
			this->label29 = (gcnew System::Windows::Forms::Label());
			this->label30 = (gcnew System::Windows::Forms::Label());
			this->label31 = (gcnew System::Windows::Forms::Label());
			this->label32 = (gcnew System::Windows::Forms::Label());
			this->label33 = (gcnew System::Windows::Forms::Label());
			this->label34 = (gcnew System::Windows::Forms::Label());
			this->label35 = (gcnew System::Windows::Forms::Label());
			this->label36 = (gcnew System::Windows::Forms::Label());
			this->label37 = (gcnew System::Windows::Forms::Label());
			this->label38 = (gcnew System::Windows::Forms::Label());
			this->label39 = (gcnew System::Windows::Forms::Label());
			this->label40 = (gcnew System::Windows::Forms::Label());
			this->label41 = (gcnew System::Windows::Forms::Label());
			this->label42 = (gcnew System::Windows::Forms::Label());
			this->label43 = (gcnew System::Windows::Forms::Label());
			this->label44 = (gcnew System::Windows::Forms::Label());
			this->label45 = (gcnew System::Windows::Forms::Label());
			this->label46 = (gcnew System::Windows::Forms::Label());
			this->label47 = (gcnew System::Windows::Forms::Label());
			this->label48 = (gcnew System::Windows::Forms::Label());
			this->label49 = (gcnew System::Windows::Forms::Label());
			this->label50 = (gcnew System::Windows::Forms::Label());
			this->label51 = (gcnew System::Windows::Forms::Label());
			this->label52 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			this->chart1->Location = System::Drawing::Point(35, 45);
			this->chart1->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->chart1->Name = L"chart1";
			this->chart1->Size = System::Drawing::Size(860, 751);
			this->chart1->TabIndex = 1;
			this->chart1->Click += gcnew System::EventHandler(this, &Chart3DControl::chart1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(1079, 339);
			this->button2->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(100, 29);
			this->button2->TabIndex = 2;
			this->button2->Text = L"上";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Chart3DControl::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(1078, 397);
			this->button3->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 29);
			this->button3->TabIndex = 3;
			this->button3->Text = L"下";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Chart3DControl::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(1028, 367);
			this->button4->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(100, 29);
			this->button4->TabIndex = 5;
			this->button4->Text = L"左";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Chart3DControl::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(1129, 367);
			this->button5->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(100, 29);
			this->button5->TabIndex = 4;
			this->button5->Text = L"右";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Chart3DControl::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(1079, 442);
			this->button6->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(100, 29);
			this->button6->TabIndex = 7;
			this->button6->Text = L"2D";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Chart3DControl::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(1079, 478);
			this->button7->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(100, 29);
			this->button7->TabIndex = 8;
			this->button7->Text = L"3D";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &Chart3DControl::button7_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 500;
			this->timer1->Tick += gcnew System::EventHandler(this, &Chart3DControl::timer1_Tick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(1078, 219);
			this->button1->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(100, 29);
			this->button1->TabIndex = 9;
			this->button1->Text = L"上段";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Chart3DControl::button1_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(1078, 256);
			this->button8->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(100, 29);
			this->button8->TabIndex = 10;
			this->button8->Text = L"中段";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &Chart3DControl::button8_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(1078, 293);
			this->button9->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(100, 29);
			this->button9->TabIndex = 11;
			this->button9->Text = L"下段";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &Chart3DControl::button9_Click);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::Red;
			this->panel1->Location = System::Drawing::Point(902, 46);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(10, 49);
			this->panel1->TabIndex = 13;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::Red;
			this->panel2->Location = System::Drawing::Point(902, 96);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(10, 49);
			this->panel2->TabIndex = 14;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::Red;
			this->panel3->Location = System::Drawing::Point(902, 146);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(10, 49);
			this->panel3->TabIndex = 15;
			this->panel3->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Chart3DControl::panel3_Paint);
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::Red;
			this->panel4->Location = System::Drawing::Point(902, 196);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(10, 49);
			this->panel4->TabIndex = 15;
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::Red;
			this->panel5->Location = System::Drawing::Point(902, 246);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(10, 49);
			this->panel5->TabIndex = 15;
			// 
			// panel6
			// 
			this->panel6->BackColor = System::Drawing::Color::Red;
			this->panel6->Location = System::Drawing::Point(902, 296);
			this->panel6->Name = L"panel6";
			this->panel6->Size = System::Drawing::Size(10, 49);
			this->panel6->TabIndex = 15;
			// 
			// panel7
			// 
			this->panel7->BackColor = System::Drawing::Color::Red;
			this->panel7->Location = System::Drawing::Point(902, 346);
			this->panel7->Name = L"panel7";
			this->panel7->Size = System::Drawing::Size(10, 49);
			this->panel7->TabIndex = 15;
			// 
			// panel8
			// 
			this->panel8->BackColor = System::Drawing::Color::Red;
			this->panel8->Location = System::Drawing::Point(902, 396);
			this->panel8->Name = L"panel8";
			this->panel8->Size = System::Drawing::Size(10, 49);
			this->panel8->TabIndex = 19;
			// 
			// panel9
			// 
			this->panel9->BackColor = System::Drawing::Color::Red;
			this->panel9->Location = System::Drawing::Point(902, 446);
			this->panel9->Name = L"panel9";
			this->panel9->Size = System::Drawing::Size(10, 49);
			this->panel9->TabIndex = 20;
			// 
			// panel10
			// 
			this->panel10->BackColor = System::Drawing::Color::Red;
			this->panel10->Location = System::Drawing::Point(902, 496);
			this->panel10->Name = L"panel10";
			this->panel10->Size = System::Drawing::Size(10, 49);
			this->panel10->TabIndex = 21;
			// 
			// panel11
			// 
			this->panel11->BackColor = System::Drawing::Color::Red;
			this->panel11->Location = System::Drawing::Point(902, 546);
			this->panel11->Name = L"panel11";
			this->panel11->Size = System::Drawing::Size(10, 49);
			this->panel11->TabIndex = 22;
			// 
			// panel12
			// 
			this->panel12->BackColor = System::Drawing::Color::Red;
			this->panel12->Location = System::Drawing::Point(902, 596);
			this->panel12->Name = L"panel12";
			this->panel12->Size = System::Drawing::Size(10, 49);
			this->panel12->TabIndex = 18;
			// 
			// panel13
			// 
			this->panel13->BackColor = System::Drawing::Color::Red;
			this->panel13->Location = System::Drawing::Point(902, 646);
			this->panel13->Name = L"panel13";
			this->panel13->Size = System::Drawing::Size(10, 49);
			this->panel13->TabIndex = 17;
			// 
			// panel14
			// 
			this->panel14->BackColor = System::Drawing::Color::Red;
			this->panel14->Location = System::Drawing::Point(902, 696);
			this->panel14->Name = L"panel14";
			this->panel14->Size = System::Drawing::Size(10, 49);
			this->panel14->TabIndex = 16;
			// 
			// panel15
			// 
			this->panel15->BackColor = System::Drawing::Color::Red;
			this->panel15->Location = System::Drawing::Point(902, 746);
			this->panel15->Name = L"panel15";
			this->panel15->Size = System::Drawing::Size(10, 49);
			this->panel15->TabIndex = 23;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(918, 63);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(63, 15);
			this->label1->TabIndex = 24;
			this->label1->Text = L"280-300";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(918, 113);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(63, 15);
			this->label2->TabIndex = 25;
			this->label2->Text = L"260-280";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(918, 163);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(63, 15);
			this->label3->TabIndex = 26;
			this->label3->Text = L"240-260";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(918, 213);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(63, 15);
			this->label4->TabIndex = 27;
			this->label4->Text = L"220-240";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(918, 263);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(63, 15);
			this->label5->TabIndex = 28;
			this->label5->Text = L"200-220";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(918, 313);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(63, 15);
			this->label6->TabIndex = 29;
			this->label6->Text = L"180-200";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(918, 363);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(63, 15);
			this->label7->TabIndex = 30;
			this->label7->Text = L"160-180";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(918, 413);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(63, 15);
			this->label8->TabIndex = 31;
			this->label8->Text = L"140-160";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(918, 463);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(63, 15);
			this->label9->TabIndex = 32;
			this->label9->Text = L"120-140";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(918, 513);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(63, 15);
			this->label10->TabIndex = 33;
			this->label10->Text = L"100-120";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(918, 563);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(55, 15);
			this->label11->TabIndex = 34;
			this->label11->Text = L"80-100";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(918, 613);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(47, 15);
			this->label12->TabIndex = 35;
			this->label12->Text = L"60-80";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(918, 663);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(47, 15);
			this->label13->TabIndex = 36;
			this->label13->Text = L"40-60";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(918, 713);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(47, 15);
			this->label14->TabIndex = 37;
			this->label14->Text = L"20-40";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(918, 763);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(39, 15);
			this->label15->TabIndex = 38;
			this->label15->Text = L"0-20";
			// 
			// label16
			// 
			this->label16->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label16->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label16->Location = System::Drawing::Point(79, 841);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(61, 30);
			this->label16->TabIndex = 39;
			this->label16->Text = L"300.0";
			this->label16->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label17
			// 
			this->label17->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label17->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label17->Location = System::Drawing::Point(142, 841);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(61, 30);
			this->label17->TabIndex = 40;
			this->label17->Text = L"300.0";
			this->label17->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label18
			// 
			this->label18->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label18->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label18->Location = System::Drawing::Point(205, 841);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(61, 30);
			this->label18->TabIndex = 41;
			this->label18->Text = L"300.0";
			this->label18->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label19
			// 
			this->label19->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label19->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label19->Location = System::Drawing::Point(268, 841);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(61, 30);
			this->label19->TabIndex = 42;
			this->label19->Text = L"300.0";
			this->label19->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label20
			// 
			this->label20->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label20->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label20->Location = System::Drawing::Point(331, 841);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(61, 30);
			this->label20->TabIndex = 43;
			this->label20->Text = L"300.0";
			this->label20->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label21
			// 
			this->label21->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label21->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label21->Location = System::Drawing::Point(394, 841);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(61, 30);
			this->label21->TabIndex = 44;
			this->label21->Text = L"300.0";
			this->label21->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label22
			// 
			this->label22->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label22->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label22->Location = System::Drawing::Point(457, 841);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(61, 30);
			this->label22->TabIndex = 45;
			this->label22->Text = L"300.0";
			this->label22->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label23
			// 
			this->label23->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label23->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label23->Location = System::Drawing::Point(520, 841);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(61, 30);
			this->label23->TabIndex = 46;
			this->label23->Text = L"300.0";
			this->label23->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label24
			// 
			this->label24->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label24->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label24->Location = System::Drawing::Point(583, 841);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(61, 30);
			this->label24->TabIndex = 47;
			this->label24->Text = L"300.0";
			this->label24->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label25
			// 
			this->label25->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label25->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label25->Location = System::Drawing::Point(646, 841);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(61, 30);
			this->label25->TabIndex = 48;
			this->label25->Text = L"300.0";
			this->label25->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label26
			// 
			this->label26->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label26->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label26->Location = System::Drawing::Point(709, 841);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(61, 30);
			this->label26->TabIndex = 49;
			this->label26->Text = L"300.0";
			this->label26->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label27
			// 
			this->label27->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label27->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label27->Location = System::Drawing::Point(772, 841);
			this->label27->Name = L"label27";
			this->label27->Size = System::Drawing::Size(61, 30);
			this->label27->TabIndex = 50;
			this->label27->Text = L"300.0";
			this->label27->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label28
			// 
			this->label28->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label28->Location = System::Drawing::Point(772, 810);
			this->label28->Name = L"label28";
			this->label28->Size = System::Drawing::Size(61, 30);
			this->label28->TabIndex = 62;
			this->label28->Text = L"#99";
			this->label28->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label29
			// 
			this->label29->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label29->Location = System::Drawing::Point(709, 810);
			this->label29->Name = L"label29";
			this->label29->Size = System::Drawing::Size(61, 30);
			this->label29->TabIndex = 61;
			this->label29->Text = L"#99";
			this->label29->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label30
			// 
			this->label30->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label30->Location = System::Drawing::Point(646, 810);
			this->label30->Name = L"label30";
			this->label30->Size = System::Drawing::Size(61, 30);
			this->label30->TabIndex = 60;
			this->label30->Text = L"#99";
			this->label30->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label31
			// 
			this->label31->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label31->Location = System::Drawing::Point(583, 810);
			this->label31->Name = L"label31";
			this->label31->Size = System::Drawing::Size(61, 30);
			this->label31->TabIndex = 59;
			this->label31->Text = L"#99";
			this->label31->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label32
			// 
			this->label32->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label32->Location = System::Drawing::Point(520, 810);
			this->label32->Name = L"label32";
			this->label32->Size = System::Drawing::Size(61, 30);
			this->label32->TabIndex = 58;
			this->label32->Text = L"#99";
			this->label32->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label33
			// 
			this->label33->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label33->Location = System::Drawing::Point(457, 810);
			this->label33->Name = L"label33";
			this->label33->Size = System::Drawing::Size(61, 30);
			this->label33->TabIndex = 57;
			this->label33->Text = L"#99";
			this->label33->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label34
			// 
			this->label34->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label34->Location = System::Drawing::Point(394, 810);
			this->label34->Name = L"label34";
			this->label34->Size = System::Drawing::Size(61, 30);
			this->label34->TabIndex = 56;
			this->label34->Text = L"#99";
			this->label34->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label35
			// 
			this->label35->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label35->Location = System::Drawing::Point(331, 810);
			this->label35->Name = L"label35";
			this->label35->Size = System::Drawing::Size(61, 30);
			this->label35->TabIndex = 55;
			this->label35->Text = L"#99";
			this->label35->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label36
			// 
			this->label36->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label36->Location = System::Drawing::Point(268, 810);
			this->label36->Name = L"label36";
			this->label36->Size = System::Drawing::Size(61, 30);
			this->label36->TabIndex = 54;
			this->label36->Text = L"#99";
			this->label36->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label37
			// 
			this->label37->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label37->Location = System::Drawing::Point(205, 810);
			this->label37->Name = L"label37";
			this->label37->Size = System::Drawing::Size(61, 30);
			this->label37->TabIndex = 53;
			this->label37->Text = L"#99";
			this->label37->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label38
			// 
			this->label38->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label38->Location = System::Drawing::Point(142, 810);
			this->label38->Name = L"label38";
			this->label38->Size = System::Drawing::Size(61, 30);
			this->label38->TabIndex = 52;
			this->label38->Text = L"#99";
			this->label38->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label39
			// 
			this->label39->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label39->Location = System::Drawing::Point(79, 810);
			this->label39->Name = L"label39";
			this->label39->Size = System::Drawing::Size(61, 30);
			this->label39->TabIndex = 51;
			this->label39->Text = L"#99";
			this->label39->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label40
			// 
			this->label40->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label40->Location = System::Drawing::Point(1010, 577);
			this->label40->Name = L"label40";
			this->label40->Size = System::Drawing::Size(101, 30);
			this->label40->TabIndex = 64;
			this->label40->Text = L"鋳込速度";
			this->label40->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label41
			// 
			this->label41->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label41->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label41->Location = System::Drawing::Point(1116, 577);
			this->label41->Name = L"label41";
			this->label41->Size = System::Drawing::Size(79, 30);
			this->label41->TabIndex = 63;
			this->label41->Text = L"300.0";
			this->label41->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label42
			// 
			this->label42->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label42->Location = System::Drawing::Point(1201, 577);
			this->label42->Name = L"label42";
			this->label42->Size = System::Drawing::Size(65, 30);
			this->label42->TabIndex = 65;
			this->label42->Text = L"m/min";
			this->label42->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label43
			// 
			this->label43->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label43->Location = System::Drawing::Point(1201, 607);
			this->label43->Name = L"label43";
			this->label43->Size = System::Drawing::Size(65, 30);
			this->label43->TabIndex = 68;
			this->label43->Text = L"mm";
			this->label43->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label44
			// 
			this->label44->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label44->Location = System::Drawing::Point(1014, 607);
			this->label44->Name = L"label44";
			this->label44->Size = System::Drawing::Size(97, 30);
			this->label44->TabIndex = 67;
			this->label44->Text = L"SN開度";
			this->label44->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label45
			// 
			this->label45->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label45->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label45->Location = System::Drawing::Point(1116, 607);
			this->label45->Name = L"label45";
			this->label45->Size = System::Drawing::Size(79, 30);
			this->label45->TabIndex = 66;
			this->label45->Text = L"300";
			this->label45->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label46
			// 
			this->label46->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label46->Location = System::Drawing::Point(1201, 637);
			this->label46->Name = L"label46";
			this->label46->Size = System::Drawing::Size(65, 30);
			this->label46->TabIndex = 71;
			this->label46->Text = L"mm";
			this->label46->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label47
			// 
			this->label47->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label47->Location = System::Drawing::Point(1014, 637);
			this->label47->Name = L"label47";
			this->label47->Size = System::Drawing::Size(97, 30);
			this->label47->TabIndex = 70;
			this->label47->Text = L"湯面レベル";
			this->label47->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label48
			// 
			this->label48->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label48->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label48->Location = System::Drawing::Point(1116, 637);
			this->label48->Name = L"label48";
			this->label48->Size = System::Drawing::Size(79, 30);
			this->label48->TabIndex = 69;
			this->label48->Text = L"300.0";
			this->label48->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label49
			// 
			this->label49->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label49->Location = System::Drawing::Point(1201, 667);
			this->label49->Name = L"label49";
			this->label49->Size = System::Drawing::Size(65, 30);
			this->label49->TabIndex = 74;
			this->label49->Text = L"m";
			this->label49->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label50
			// 
			this->label50->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label50->Location = System::Drawing::Point(1014, 667);
			this->label50->Name = L"label50";
			this->label50->Size = System::Drawing::Size(97, 30);
			this->label50->TabIndex = 73;
			this->label50->Text = L"鋳込長";
			this->label50->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// label51
			// 
			this->label51->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->label51->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label51->Location = System::Drawing::Point(1116, 667);
			this->label51->Name = L"label51";
			this->label51->Size = System::Drawing::Size(79, 30);
			this->label51->TabIndex = 72;
			this->label51->Text = L"300.0";
			this->label51->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label52
			// 
			this->label52->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label52->Location = System::Drawing::Point(31, 11);
			this->label52->Name = L"label52";
			this->label52->Size = System::Drawing::Size(77, 30);
			this->label52->TabIndex = 75;
			this->label52->Text = L"上段";
			this->label52->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// Chart3DControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->Controls->Add(this->label52);
			this->Controls->Add(this->label49);
			this->Controls->Add(this->label50);
			this->Controls->Add(this->label51);
			this->Controls->Add(this->label46);
			this->Controls->Add(this->label47);
			this->Controls->Add(this->label48);
			this->Controls->Add(this->label43);
			this->Controls->Add(this->label44);
			this->Controls->Add(this->label45);
			this->Controls->Add(this->label42);
			this->Controls->Add(this->label40);
			this->Controls->Add(this->label41);
			this->Controls->Add(this->label28);
			this->Controls->Add(this->label29);
			this->Controls->Add(this->label30);
			this->Controls->Add(this->label31);
			this->Controls->Add(this->label32);
			this->Controls->Add(this->label33);
			this->Controls->Add(this->label34);
			this->Controls->Add(this->label35);
			this->Controls->Add(this->label36);
			this->Controls->Add(this->label37);
			this->Controls->Add(this->label38);
			this->Controls->Add(this->label39);
			this->Controls->Add(this->label27);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label23);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->panel15);
			this->Controls->Add(this->panel8);
			this->Controls->Add(this->panel7);
			this->Controls->Add(this->panel9);
			this->Controls->Add(this->panel6);
			this->Controls->Add(this->panel10);
			this->Controls->Add(this->panel5);
			this->Controls->Add(this->panel11);
			this->Controls->Add(this->panel4);
			this->Controls->Add(this->panel12);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel13);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel14);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->chart1);
			this->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->Margin = System::Windows::Forms::Padding(4, 4, 4, 4);
			this->Name = L"Chart3DControl";
			this->Size = System::Drawing::Size(1280, 925);
			this->Load += gcnew System::EventHandler(this, &Chart3DControl::Chart3DControl_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Chart3DControl_Load(System::Object^  sender, System::EventArgs^  e) 
			{
				// パス設定
				szAppPath = Application::StartupPath;
				szTcNameFile = szAppPath + "/../Config/TcName.dat";

				// 基本設定
				this->chart1->Antialiasing = false;
				this->chart1->Gallery = Gallery::Surface;
				this->chart1->AllSeries->MultipleColors = true;
				this->chart1->AllSeries->FillMode = FillMode::Solid; 
				this->chart1->Data->Series = 12;
				this->chart1->Data->Points = 100;
				this->chart1->LegendBox->Visible = false;
//				this->chart1->LegendBox->Dock = DockArea::Right;
//				this->chart1->LegendBox->Border = ChartFX::WinForms::DockBorder::External;

				String	^str;
				int		cnt = 0;

				array<String^>^ s1;
				try {
					s1 = System::IO::File::ReadAllLines(szTcNameFile, System::Text::Encoding::Default);
					for each (String^ item in s1)
					{
						myTcName[cnt] = item;
						cnt++;
					}
				}
				catch(Exception^ e)
				{
				}
				for( ; cnt < 28; cnt++ )
					myTcName[cnt] = "";

				// 項目名
				for( int i = 0; i < 12; i++ )
				{
					if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
						str = "";
					else
						str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
					this->chart1->Series[i]->Text = str;
				}

				// データ表示ラベル
				label28->Text = this->chart1->Series[0]->Text;
				label29->Text = this->chart1->Series[1]->Text;
				label30->Text = this->chart1->Series[2]->Text;
				label31->Text = this->chart1->Series[3]->Text;
				label32->Text = this->chart1->Series[4]->Text;
				label33->Text = this->chart1->Series[5]->Text;
				label34->Text = this->chart1->Series[6]->Text;
				label35->Text = this->chart1->Series[7]->Text;
				label36->Text = this->chart1->Series[8]->Text;
				label37->Text = this->chart1->Series[9]->Text;
				label38->Text = this->chart1->Series[10]->Text;
				label39->Text = this->chart1->Series[11]->Text;

				// 時間軸
				this->chart1->AxisX->Step = 5;
				this->chart1->AxisX->FirstLabel = 1;
				for( int i = 0; i < 100; i++ )
				{
					if( i % 10 == 0 )
					{
						str = (i*0.5).ToString("0");
						this->chart1->AxisX->Labels[i] = str;
					}
				}
				this->chart1->AxisX->Labels[95] = "[sec]";
				this->chart1->AxisX->LabelAngle = 0;
				this->chart1->AxisX->Position = AxisPosition::Near;
				this->chart1->AxisX->Font = gcnew System::Drawing::Font("MS UI Gothic", 9);

				// 色の設定
				array<Color>^ myColor = gcnew array<Color>(15);
				myColor[ 0] = Color::FromArgb( 55, 55,255);
				myColor[ 1] = Color::FromArgb( 55,155,255);
				myColor[ 2] = Color::FromArgb( 55,255,255);
				myColor[ 3] = Color::FromArgb( 55,255,155);
				myColor[ 4] = Color::FromArgb( 55,255, 55);
				myColor[ 5] = Color::FromArgb(155,255, 55);
				myColor[ 6] = Color::FromArgb(255,255, 55);
				myColor[ 7] = Color::FromArgb(255,230, 55);
				myColor[ 8] = Color::FromArgb(255,205, 55);
				myColor[ 9] = Color::FromArgb(255,180, 55);
				myColor[10] = Color::FromArgb(255,155, 55);
				myColor[11] = Color::FromArgb(255,130, 55);
				myColor[12] = Color::FromArgb(255,105, 55);
				myColor[13] = Color::FromArgb(255, 80, 55);
				myColor[14] = Color::FromArgb(255, 55, 55);
				Galleries::Surface ^mySurface = (Galleries::Surface ^)this->chart1->GalleryAttributes;
				mySurface->Step = 20;
				mySurface->Colors = myColor;
				mySurface->ShowGridLines = true;
				
				// パネルの設定
				panel1->BackColor = myColor[14];
				panel2->BackColor = myColor[13];
				panel3->BackColor = myColor[12];
				panel4->BackColor = myColor[11];
				panel5->BackColor = myColor[10];
				panel6->BackColor = myColor[ 9];
				panel7->BackColor = myColor[ 8];
				panel8->BackColor = myColor[ 7];
				panel9->BackColor = myColor[ 6];
				panel10->BackColor = myColor[ 5];
				panel11->BackColor = myColor[ 4];
				panel12->BackColor = myColor[ 3];
				panel13->BackColor = myColor[ 2];
				panel14->BackColor = myColor[ 1];
				panel15->BackColor = myColor[ 0];

				// 3D Setting
				this->chart1->View3D->Enabled = true;
				this->chart1->View3D->Depth = 600;
				this->chart1->View3D->AngleX = 10;
				this->chart1->View3D->AngleY = 290;
				this->chart1->View3D->Perspective = 0;

				//Constrains
				this->chart1->ToolTips = false;

				///////////////////////////////////////////////////////////////
				// Surface

				this->chart1->PlotAreaMargin->Left = 30;
				this->chart1->PlotAreaMargin->Right = 30;
				this->chart1->PlotAreaMargin->Top = 0;
				this->chart1->PlotAreaMargin->Bottom = 20;

				// color
				this->chart1->AxisY->Style |= AxisStyles::BreakZero;
				this->chart1->BackColor = Color::White;
				this->chart1->AxisY->Step = 20;
				this->chart1->AxisY->Min = 0;
				this->chart1->AxisY->Max = 300;
				this->chart1->AxisY->Title->Text = "[C]";
				this->chart1->AxisX->Title->Text = "[TC]";

			}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
 				this->chart1->View3D->AngleX += 5;
			}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
 				this->chart1->View3D->AngleX -= 5;
			}
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e) {
 				this->chart1->View3D->AngleY -= 5;
			}

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
 				this->chart1->View3D->AngleY += 5;
			}
	private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e) {

			    isDraw = false;

				// 基本設定
				this->chart1->Gallery = Gallery::Contour;
				this->chart1->AllSeries->MultipleColors = true;
				this->chart1->AllSeries->FillMode = FillMode::Solid; 
				this->chart1->Data->Series = 100;
				this->chart1->Data->Points = 12;
				this->chart1->LegendBox->Visible = false;
//				this->chart1->LegendBox->Dock = DockArea::Right;
//				this->chart1->LegendBox->Border = ChartFX::WinForms::DockBorder::External;

				String	^str;

				// 項目名
				this->chart1->AxisX->Step = 1;
				for( int i = 0; i < 12; i++ )
				{
					if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
						str = "";
					else
						str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
					this->chart1->AxisX->Labels[12-i-1] = str;
				}
				this->chart1->AxisX->LabelAngle = 0;
				this->chart1->AxisX->Position = AxisPosition::Near;
//				this->chart1->AxisX->Font = gcnew System::Drawing::Font("MS UI Gothic", 9);

				//// 時間軸
				for( int i = 0; i < 100; i++ )
				{
					if( i % 10 == 0 )
					{
						str = String::Format("{0:D}", (int)(i*0.5));
						this->chart1->Series[i]->Text = str;
					}
					else
						this->chart1->Series[i]->Text = "";
				}
				this->chart1->Series[95]->Text = "[sec]";

				// 色の設定
				array<Color>^ myColor = gcnew array<Color>(15);
				myColor[ 0] = Color::FromArgb( 55, 55,255);
				myColor[ 1] = Color::FromArgb( 55,155,255);
				myColor[ 2] = Color::FromArgb( 55,255,255);
				myColor[ 3] = Color::FromArgb( 55,255,155);
				myColor[ 4] = Color::FromArgb( 55,255, 55);
				myColor[ 5] = Color::FromArgb(155,255, 55);
				myColor[ 6] = Color::FromArgb(255,255, 55);
				myColor[ 7] = Color::FromArgb(255,230, 55);
				myColor[ 8] = Color::FromArgb(255,205, 55);
				myColor[ 9] = Color::FromArgb(255,180, 55);
				myColor[10] = Color::FromArgb(255,155, 55);
				myColor[11] = Color::FromArgb(255,130, 55);
				myColor[12] = Color::FromArgb(255,105, 55);
				myColor[13] = Color::FromArgb(255, 80, 55);
				myColor[14] = Color::FromArgb(255, 55, 55);
				Galleries::Surface ^mySurface = (Galleries::Surface ^)this->chart1->GalleryAttributes;
				mySurface->Step = 20;
				mySurface->Colors = myColor;
				

				// 3D Setting
				this->chart1->View3D->Enabled = false;

				//Constrains
				this->chart1->ToolTips = false;

				///////////////////////////////////////////////////////////////
				// Surface

				this->chart1->PlotAreaMargin->Left = 30;
				this->chart1->PlotAreaMargin->Right = 30;
				this->chart1->PlotAreaMargin->Top = 0;
				this->chart1->PlotAreaMargin->Bottom = 20;

				// color
				this->chart1->AxisY->Style &= ~AxisStyles::BreakZero;
				this->chart1->BackColor = Color::White;
				this->chart1->AxisY->Step = 20;
				this->chart1->AxisY->Min = 0;
				this->chart1->AxisY->Max = 300;
				this->chart1->AxisY->Title->Text = "";

				isSurface = false;
			    isDraw = true;
			}
private: System::Void chart1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e) {
			    isDraw = false;

				// 基本設定
				this->chart1->Antialiasing = false;
				this->chart1->Gallery = Gallery::Surface;
				this->chart1->AllSeries->MultipleColors = true;
				this->chart1->AllSeries->FillMode = FillMode::Solid; 
				this->chart1->Data->Series = 12;
				this->chart1->Data->Points = 100;
				this->chart1->LegendBox->Visible = false;
//				this->chart1->LegendBox->Dock = DockArea::Right;
//				this->chart1->LegendBox->Border = ChartFX::WinForms::DockBorder::External;

				String	^str;

				// 項目名
				for( int i = 0; i < 12; i++ )
				{
					if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
						str = "";
					else
						str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
					this->chart1->Series[i]->Text = str;
				}

				// データ表示ラベル
				label28->Text = this->chart1->Series[0]->Text;
				label29->Text = this->chart1->Series[1]->Text;
				label30->Text = this->chart1->Series[2]->Text;
				label31->Text = this->chart1->Series[3]->Text;
				label32->Text = this->chart1->Series[4]->Text;
				label33->Text = this->chart1->Series[5]->Text;
				label34->Text = this->chart1->Series[6]->Text;
				label35->Text = this->chart1->Series[7]->Text;
				label36->Text = this->chart1->Series[8]->Text;
				label37->Text = this->chart1->Series[9]->Text;
				label38->Text = this->chart1->Series[10]->Text;
				label39->Text = this->chart1->Series[11]->Text;

				// 時間軸
				this->chart1->AxisX->Step = 5;
				this->chart1->AxisX->FirstLabel = 1;
				for( int i = 0; i < 100; i++ )
				{
					if( i % 10 == 0 )
					{
						str = (i*0.5).ToString("0");
						this->chart1->AxisX->Labels[i] = str;
					}
					else
						this->chart1->AxisX->Labels[i] = "";
				}
				this->chart1->AxisX->Labels[95] = "[sec]";
				this->chart1->AxisX->LabelAngle = 0;
				this->chart1->AxisX->Position = AxisPosition::Near;
				this->chart1->AxisX->Font = gcnew System::Drawing::Font("MS UI Gothic", 9);

				// 色の設定
				array<Color>^ myColor = gcnew array<Color>(15);
				myColor[ 0] = Color::FromArgb( 55, 55,255);
				myColor[ 1] = Color::FromArgb( 55,155,255);
				myColor[ 2] = Color::FromArgb( 55,255,255);
				myColor[ 3] = Color::FromArgb( 55,255,155);
				myColor[ 4] = Color::FromArgb( 55,255, 55);
				myColor[ 5] = Color::FromArgb(155,255, 55);
				myColor[ 6] = Color::FromArgb(255,255, 55);
				myColor[ 7] = Color::FromArgb(255,230, 55);
				myColor[ 8] = Color::FromArgb(255,205, 55);
				myColor[ 9] = Color::FromArgb(255,180, 55);
				myColor[10] = Color::FromArgb(255,155, 55);
				myColor[11] = Color::FromArgb(255,130, 55);
				myColor[12] = Color::FromArgb(255,105, 55);
				myColor[13] = Color::FromArgb(255, 80, 55);
				myColor[14] = Color::FromArgb(255, 55, 55);
				Galleries::Surface ^mySurface = (Galleries::Surface ^)this->chart1->GalleryAttributes;
				mySurface->Step = 20;
				mySurface->Colors = myColor;
				
				// パネルの設定
				panel1->BackColor = myColor[14];
				panel2->BackColor = myColor[13];
				panel3->BackColor = myColor[12];
				panel4->BackColor = myColor[11];
				panel5->BackColor = myColor[10];
				panel6->BackColor = myColor[ 9];
				panel7->BackColor = myColor[ 8];
				panel8->BackColor = myColor[ 7];
				panel9->BackColor = myColor[ 6];
				panel10->BackColor = myColor[ 5];
				panel11->BackColor = myColor[ 4];
				panel12->BackColor = myColor[ 3];
				panel13->BackColor = myColor[ 2];
				panel14->BackColor = myColor[ 1];
				panel15->BackColor = myColor[ 0];

				// 3D Setting
				this->chart1->View3D->Enabled = true;
				this->chart1->View3D->Depth = 600;
				this->chart1->View3D->AngleX = 10;
				this->chart1->View3D->AngleY = 290;
				this->chart1->View3D->Perspective = 0;

				//Constrains
				this->chart1->ToolTips = false;

				///////////////////////////////////////////////////////////////
				// Surface

				this->chart1->PlotAreaMargin->Left = 30;
				this->chart1->PlotAreaMargin->Right = 30;
				this->chart1->PlotAreaMargin->Top = 0;
				this->chart1->PlotAreaMargin->Bottom = 20;

				// color
				this->chart1->AxisY->Style |= AxisStyles::BreakZero;
				this->chart1->BackColor = Color::White;
				this->chart1->AxisY->Step = 20;
				this->chart1->AxisY->Min = 0;
				this->chart1->AxisY->Max = 300;
				this->chart1->AxisY->Title->Text = "[C]";
				this->chart1->AxisX->Title->Text = "[TC]";

				isSurface = true;
			    isDraw = true;
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				
				// 描画する
				if( isDraw )
				{
					// tc name変更要求ありなら
					if( reqChgTcName )
					{
						String^			str;
						int				cnt = 0;
						array<String^>^ s1;

						try {
							s1 = System::IO::File::ReadAllLines(szTcNameFile, System::Text::Encoding::Default);
							for each (String^ item in s1)
							{
								myTcName[cnt] = item;
								cnt++;
							}
						}
						catch(Exception^ e)
						{
						}
						for( ; cnt < 28; cnt++ )
							myTcName[cnt] = "";

						// 項目名
						for( int i = 0; i < 12; i++ )
						{
							if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
								str = "";
							else
								str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
							this->chart1->Series[i]->Text = str;
						}

						// データ表示ラベル
						label28->Text = this->chart1->Series[0]->Text;
						label29->Text = this->chart1->Series[1]->Text;
						label30->Text = this->chart1->Series[2]->Text;
						label31->Text = this->chart1->Series[3]->Text;
						label32->Text = this->chart1->Series[4]->Text;
						label33->Text = this->chart1->Series[5]->Text;
						label34->Text = this->chart1->Series[6]->Text;
						label35->Text = this->chart1->Series[7]->Text;
						label36->Text = this->chart1->Series[8]->Text;
						label37->Text = this->chart1->Series[9]->Text;
						label38->Text = this->chart1->Series[10]->Text;
						label39->Text = this->chart1->Series[11]->Text;

						reqChgTcName = false;
					}

					if( nLineNo == 0 )
					{
						for(int i = 0; i < 12; i++ )
							for( int j = 0; j < 100; j++ )
							{
								if( isSurface )
									this->chart1->Data[i,j] = myData1[11-i,j]/10.0;
								else
									this->chart1->Data[j,i] = myData1[i,j]/10.0;
							}
					}
					if( nLineNo == 1 )
					{
						for(int i = 0; i < 12; i++ )
							for( int j = 0; j < 100; j++ )
							{
								if( isSurface )
									this->chart1->Data[i,j] = myData2[11-i,j]/10.0;
								else
									this->chart1->Data[j,i] = myData2[i,j]/10.0;
							}
					}
					if( nLineNo == 2 )
					{
						for(int i = 0; i < 12; i++ )
							for( int j = 0; j < 100; j++ )
							{
								if( isSurface )
									this->chart1->Data[i,j] = myData3[11-i,j]/10.0;
								else
									this->chart1->Data[j,i] = myData3[i,j]/10.0;
							}
					}

					if( isSurface )
					{
						label27->Text = this->chart1->Data[0,0].ToString("0.0");
						label26->Text = this->chart1->Data[1,0].ToString("0.0");
						label25->Text = this->chart1->Data[2,0].ToString("0.0");
						label24->Text = this->chart1->Data[3,0].ToString("0.0");
						label23->Text = this->chart1->Data[4,0].ToString("0.0");
						label22->Text = this->chart1->Data[5,0].ToString("0.0");
						label21->Text = this->chart1->Data[6,0].ToString("0.0");
						label20->Text = this->chart1->Data[7,0].ToString("0.0");
						label19->Text = this->chart1->Data[8,0].ToString("0.0");
						label18->Text = this->chart1->Data[9,0].ToString("0.0");
						label17->Text = this->chart1->Data[10,0].ToString("0.0");
						label16->Text = this->chart1->Data[11,0].ToString("0.0");
					}
					else
					{
						label16->Text = this->chart1->Data[0,0].ToString("0.0");
						label17->Text = this->chart1->Data[0,1].ToString("0.0");
						label18->Text = this->chart1->Data[0,2].ToString("0.0");
						label19->Text = this->chart1->Data[0,3].ToString("0.0");
						label20->Text = this->chart1->Data[0,4].ToString("0.0");
						label21->Text = this->chart1->Data[0,5].ToString("0.0");
						label22->Text = this->chart1->Data[0,6].ToString("0.0");
						label23->Text = this->chart1->Data[0,7].ToString("0.0");
						label24->Text = this->chart1->Data[0,8].ToString("0.0");
						label25->Text = this->chart1->Data[0,9].ToString("0.0");
						label26->Text = this->chart1->Data[0,10].ToString("0.0");
						label27->Text = this->chart1->Data[0,11].ToString("0.0");
					}

					label41->Text = (myDataEx[1]/100.0).ToString("0.00");
					label45->Text = (myDataEx[2]).ToString("0");
					label48->Text = (myDataEx[0]/10.0).ToString("0.0");
					label51->Text = (myDataEx[3]/100.0).ToString("0.00");
				}
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			String	^str;
 			nLineNo = 0;
			label52->Text = "上段";
			// 項目名
			for( int i = 0; i < 12; i++ )
			{
				if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
					str = "";
				else
					str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
				if( isSurface )
					this->chart1->Series[i]->Text = str;
				else
					this->chart1->AxisX->Labels[11-i] = str;
			}

			// データ表示ラベル
			if( isSurface )
			{
				label28->Text = this->chart1->Series[0]->Text;
				label29->Text = this->chart1->Series[1]->Text;
				label30->Text = this->chart1->Series[2]->Text;
				label31->Text = this->chart1->Series[3]->Text;
				label32->Text = this->chart1->Series[4]->Text;
				label33->Text = this->chart1->Series[5]->Text;
				label34->Text = this->chart1->Series[6]->Text;
				label35->Text = this->chart1->Series[7]->Text;
				label36->Text = this->chart1->Series[8]->Text;
				label37->Text = this->chart1->Series[9]->Text;
				label38->Text = this->chart1->Series[10]->Text;
				label39->Text = this->chart1->Series[11]->Text;
			}
			else
			{
				label28->Text = this->chart1->AxisX->Labels[11];
				label29->Text = this->chart1->AxisX->Labels[10];
				label30->Text = this->chart1->AxisX->Labels[9];
				label31->Text = this->chart1->AxisX->Labels[8];
				label32->Text = this->chart1->AxisX->Labels[7];
				label33->Text = this->chart1->AxisX->Labels[6];
				label34->Text = this->chart1->AxisX->Labels[5];
				label35->Text = this->chart1->AxisX->Labels[4];
				label36->Text = this->chart1->AxisX->Labels[3];
				label37->Text = this->chart1->AxisX->Labels[2];
				label38->Text = this->chart1->AxisX->Labels[1];
				label39->Text = this->chart1->AxisX->Labels[0];
			}
		 }
private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e) {
 			String	^str;
			nLineNo = 1;
			label52->Text = "中段";
			// 項目名
			for( int i = 0; i < 12; i++ )
			{
				if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
					str = "";
				else
					str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
				if( isSurface )
					this->chart1->Series[i]->Text = str;
				else
					this->chart1->AxisX->Labels[11-i] = str;
			}

			// データ表示ラベル
			if( isSurface )
			{
				label28->Text = this->chart1->Series[0]->Text;
				label29->Text = this->chart1->Series[1]->Text;
				label30->Text = this->chart1->Series[2]->Text;
				label31->Text = this->chart1->Series[3]->Text;
				label32->Text = this->chart1->Series[4]->Text;
				label33->Text = this->chart1->Series[5]->Text;
				label34->Text = this->chart1->Series[6]->Text;
				label35->Text = this->chart1->Series[7]->Text;
				label36->Text = this->chart1->Series[8]->Text;
				label37->Text = this->chart1->Series[9]->Text;
				label38->Text = this->chart1->Series[10]->Text;
				label39->Text = this->chart1->Series[11]->Text;
			}
			else
			{
				label28->Text = this->chart1->AxisX->Labels[11];
				label29->Text = this->chart1->AxisX->Labels[10];
				label30->Text = this->chart1->AxisX->Labels[9];
				label31->Text = this->chart1->AxisX->Labels[8];
				label32->Text = this->chart1->AxisX->Labels[7];
				label33->Text = this->chart1->AxisX->Labels[6];
				label34->Text = this->chart1->AxisX->Labels[5];
				label35->Text = this->chart1->AxisX->Labels[4];
				label36->Text = this->chart1->AxisX->Labels[3];
				label37->Text = this->chart1->AxisX->Labels[2];
				label38->Text = this->chart1->AxisX->Labels[1];
				label39->Text = this->chart1->AxisX->Labels[0];
			}
		 }
private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
 			String	^str;
			nLineNo = 2;
			label52->Text = "下段";
			// 項目名
			for( int i = 0; i < 12; i++ )
			{
				if( myLabel[nLineNo,i] == 0 || myLabel[nLineNo,i] > 28 )
					str = "";
				else
					str = String::Format("{0:S}", myTcName[myLabel[nLineNo,i]-1]);
				if( isSurface )
					this->chart1->Series[i]->Text = str;
				else
					this->chart1->AxisX->Labels[11-i] = str;
			}

			// データ表示ラベル
			if( isSurface )
			{
				label28->Text = this->chart1->Series[0]->Text;
				label29->Text = this->chart1->Series[1]->Text;
				label30->Text = this->chart1->Series[2]->Text;
				label31->Text = this->chart1->Series[3]->Text;
				label32->Text = this->chart1->Series[4]->Text;
				label33->Text = this->chart1->Series[5]->Text;
				label34->Text = this->chart1->Series[6]->Text;
				label35->Text = this->chart1->Series[7]->Text;
				label36->Text = this->chart1->Series[8]->Text;
				label37->Text = this->chart1->Series[9]->Text;
				label38->Text = this->chart1->Series[10]->Text;
				label39->Text = this->chart1->Series[11]->Text;
			}
			else
			{
				label28->Text = this->chart1->AxisX->Labels[11];
				label29->Text = this->chart1->AxisX->Labels[10];
				label30->Text = this->chart1->AxisX->Labels[9];
				label31->Text = this->chart1->AxisX->Labels[8];
				label32->Text = this->chart1->AxisX->Labels[7];
				label33->Text = this->chart1->AxisX->Labels[6];
				label34->Text = this->chart1->AxisX->Labels[5];
				label35->Text = this->chart1->AxisX->Labels[4];
				label36->Text = this->chart1->AxisX->Labels[3];
				label37->Text = this->chart1->AxisX->Labels[2];
				label38->Text = this->chart1->AxisX->Labels[1];
				label39->Text = this->chart1->AxisX->Labels[0];
			}
		 }
private: System::Void panel3_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
		 }
};
}
